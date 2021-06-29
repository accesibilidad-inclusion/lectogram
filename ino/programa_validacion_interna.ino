#include <SPI.h>
#include <MFRC522.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#include <Adafruit_NeoPixel.h>

#define PIN 6 //Designamos nuestro pin de datos
#define NUMPIXELS      16 //Designamos cuantos pixeles tenemos en nuestra cinta led RGB
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// IMPORTANTE: Para reducir el riesgo de dañar los pixeles de la cinta, añada un condensador de 1000 uF a través de
// los cables de alimentación de la cinta, agregue una resistencia de 300-500 Ohm en la entrada de datos del primer pixel
// Y reduzca la distancia entre Arduino y el primera pixel.


#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522

//SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
char command;
int pausa = 0;
int delayval = 50; // Pausa
uint32_t rojo = pixels.Color(150,0,0);
uint32_t verde = pixels.Color(0,150,0);
uint32_t azul = pixels.Color(0,0,150);
uint32_t apagado = pixels.Color(0,0,0);
uint32_t amarillo = pixels.Color(150,150,0);
uint32_t morado = pixels.Color(150,0,150);
uint32_t celeste = pixels.Color(0,150,150);
uint32_t blanco = pixels.Color(150,150,150);
   
void setup() 
{
pixels.begin(); 
mySoftwareSerial.begin(9600); //Comunicacao serial com o modulo
Serial.begin(115200);//Inicializa a serial do Arduino
Serial.println();//Verifica se o modulo esta respondendo e se o
Serial.println(F("DFRobot DFPlayer Mini"));
Serial.println(F("Initializing DFPlayer module ... Wait!"));
if (!myDFPlayer.begin(mySoftwareSerial))
{
  Serial.println(F("Not initialized:"));
  Serial.println(F("1. Check the DFPlayer Mini connections"));
  Serial.println(F("2. Insert an SD card"));
  // while (true); 
}
Serial.println();
Serial.println(F("DFPlayer Mini module initialized!"));
myDFPlayer.setTimeOut(500); //Timeout serial 500ms
myDFPlayer.volume(28); //Volume máx 30
myDFPlayer.EQ(5); //Equalizacao normal
//menu_opcoes();

SPI.begin();        //Iniciamos el Bus SPI
mfrc522.PCD_Init(); // Iniciamos el MFRC522
Serial.println("Control de acceso:");
command='6';                                    //bienvenido al sistema
myDFPlayer.playFolder(05,001);
delay(1000);/*audios de instrucción por carpeta*/ 

for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, blanco); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   } 

for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, blanco); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   } 
  
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Cocina[4]= {0x3B, 0xD6, 0x16, 0xEC} ; //código de cocina
byte Bano[4]= {0x3B, 0xCF, 0x16, 0xEC} ; //código del baño 
byte Bano1[4]= {0x6B, 0xCC, 0x16, 0xEC} ; //código del baño 
byte Bano2[4]= {0x5B, 0xDA, 0x16, 0xEC} ; //código del baño 

//byte Patio[4]= {0x1D, 0xC8, 0x16, 0xEC} ; //código del patio
//byte Dormitorio[4]= {0xCB, 0xC3, 0x16, 0xEC } ; //código del dormitorio 
//byte Living[4]= {0x5B, 0xBF, 0x16, 0xEC} ; //código del living

/*
 * TARJETAS
1->COCINA-> 3B D6 16 EC 
2->BAÑO 0-> 3B CF 16 EC
3->BAÑO 1-> 6B CC 16 EC 
4->BAÑO 2-> 5B DA 16 EC 
5->PATIO-> 1B C8 16 EC
6->DORMITORIO-> CB C3 16 EC 
7->LIVING->  5B BF 16 EC 
 */

void loop()
{
//---------------------------------------------------selección de audios
if ( mfrc522.PICC_IsNewCardPresent())  // Revisamos si hay nuevas tarjetas  presentes
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) 
                  {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  /*
                   * INGRESO DE USUARIOS (TARJETAS) Y SUS DISTINTOS COMMAND, QUE ACTIVARÁN LOS DISTINTOS SONIDOS
                   */
                   }
                    if(compareArray(ActualUID,Cocina)) //------------------------------------------COCINA
                    {
                   // Serial.println("Acceso concedido...");
                    command='1';
                    LED_MORADO(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
                    LED_MORADO(1);
                    myDFPlayer.playFolder(06,001); // Actividad: Preparar té
                    delay(3000);
                    LED_MORADO(4);
                    myDFPlayer.playFolder(06,002); // Actividad: Preparar té, intrucción 1
                    delay(10000);
                    LED_MORADO(8);
                    myDFPlayer.playFolder(06,003); // Actividad: Preparar té, intrucción 2
                    delay(10000);
                    LED_MORADO(12);
                    myDFPlayer.playFolder(06,004); // Actividad: Preparar té, intrucción 3
                    delay(10000);
                    LED_MORADO(16); 
                    myDFPlayer.playFolder(06,005); // Actividad: Preparar té, intrucción 4
                    delay(7000); 
                    LED_MORADO(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)    
                    }
                  else if(compareArray(ActualUID,Bano)) //-------------------------------------BAÑO
                    {
                   // Serial.println("Acceso concedido...");
                    command='2'; 
                    LED_AZUL(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB 
                    LED_AZUL(1); 
                     myDFPlayer.playFolder(03,001); // Actividad: Lavar manos
                     delay(3000);
                    LED_AZUL(4); 
                     myDFPlayer.playFolder(03,002); // Actividad: Lavar manos, intrucción 1
                     delay(10000);
                     LED_AZUL(9);
                     myDFPlayer.playFolder(03,003); // Actividad: Lavar manos, intrucción 2
                     delay(10000);
                     LED_AZUL(12);
                     myDFPlayer.playFolder(03,004); // Actividad: Lavar manos, intrucción 3
                     delay(10000); 
                     LED_AZUL(16);
                     myDFPlayer.playFolder(03,005); // Actividad: Lavar lavar manos, intrucción 4
                     delay(10000); 
                     LED_AZUL(16);
                     myDFPlayer.playFolder(03,006); // Actividad: Lavar manos, intrucción 5
                     delay(5000);
                     LED_AZUL(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB 
                   
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)                     
                    }
                    else if(compareArray(ActualUID,Bano1)) //-------------------------------------BAÑO 1
                    {
                   // Serial.println("Acceso concedido...");
                    command='2'; 
                    LED_AZUL(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB  
                    LED_AZUL(1); 
                    myDFPlayer.playFolder(02,001); // Actividad: Lavarse los dientes
                    delay(3000);
                      LED_AZUL(4); 
                    myDFPlayer.playFolder(02,002); // Actividad: Lavarse los dientes, intrucción 1
                    delay(10000);
                      LED_AZUL(8); 
                    myDFPlayer.playFolder(02,003); // Actividad: Lavarse los dientes, intrucción 2
                    delay(10000);
                      LED_AZUL(12); 
                    myDFPlayer.playFolder(02,004); // Actividad: Lavarse los dientes, intrucción 3
                    delay(10000); 
                      LED_AZUL(16); 
                    myDFPlayer.playFolder(02,005); // Actividad: Lavarse los dientes, intrucción 4
                    delay(7000);
                      LED_AZUL(16); 
                      pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta 
                      
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)                
                    }
                    else if(compareArray(ActualUID,Bano2)) //-------------------------------------BAÑO 2
                    {
                   // Serial.println("Acceso concedido...");
                    command='2';
                      LED_AZUL(16); 
                      pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta   
                    LED_AZUL(1); 
                    myDFPlayer.playFolder(04,001); // Actividad: Lavarse ropa
                    delay(3000);
                    LED_AZUL(4);
                    myDFPlayer.playFolder(04,002); // Actividad: Lavar ropa, intrucción 1
                    delay(10000);
                    LED_AZUL(6);
                    myDFPlayer.playFolder(04,003); // Actividad: Lavar ropa, intrucción 2
                    delay(10000);
                    LED_AZUL(8);
                    myDFPlayer.playFolder(04,004); // Actividad: Lavarse ropa, intrucción 3
                    delay(10000); 
                    LED_AZUL(10);
                    myDFPlayer.playFolder(04,005); // Actividad: Lavarse ropa, intrucción 4
                    delay(7000);
                    LED_AZUL(13);
                    myDFPlayer.playFolder(04,006); // Actividad: Lavar ropa, intrucción 5
                    delay(7000);
                    LED_AZUL(16);
                    myDFPlayer.playFolder(04,007); // Actividad: Lavar ropa, intrucción 6
                    delay(7000);
                    LED_AZUL(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta 
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)                  
                    } 
                   
                 /* else if(compareArray(ActualUID,Patio)) //-------------------------------------PATIO
                    {
                   // Serial.println("Acceso concedido...");
                    command='3';
                    LED_VERDE(16);
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta
                    LED_VERDE(1); 
                    myDFPlayer.playFolder(07,001); // Actividad: Regar plantas
                    delay(7000);
                    LED_VERDE(4); 
                    myDFPlayer.playFolder(07,002); // Actividad: Regar plantas, intrucción 1
                    delay(7000);
                    LED_VERDE(8); 
                    myDFPlayer.playFolder(07,003); // Actividad: Regar plantas, intrucción 2
                    delay(7000); 
                    LED_VERDE(16); 
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)                     
                    }
                   
                   else if(compareArray(ActualUID,Dormitorio)) //-------------------------------------DORMITORIO
                    {
                   // Serial.println("Acceso concedido...");
                    command='4';
                    LED_AMARILLO(16); 
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta
                     LED_AMARILLO(1); 
                     myDFPlayer.playFolder(); // Actividad
                    delay(); 
                    LED_AMARILLO(2); 
                    //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)                   
                    }
                   else if(compareArray(ActualUID,Living)) //-------------------------------------LIVING
                    {
                   // Serial.println("Acceso concedido...");
                    command='5'; 
                    LED_ROJO(16);   
                    pixels.clear(); // Brillo moderado en azul
                    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta
                     LED_ROJO(1); 
                     myDFPlayer.playFolder(); // Actividad
                    delay();    
                    LED_ROJO(2);    
                     //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)      
                    } */
                  else
                   { Serial.println("Acceso denegado...");}
                                   
                  mfrc522.PICC_HaltA(); // Terminamos la lectura de la tarjeta tarjeta  actual
              
                      
        }        
}
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

void LED_ROJO(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i, rojo); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_VERDE(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i,verde); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}

void LED_AZUL(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i, azul); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_AMARILLO(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i, amarillo); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_MORADO(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i,morado); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_CELESTE(int n_led)
{
  for(int i=0;i<n_led;i++)
   {
    pixels.setPixelColor(i, celeste); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    //pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    //pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}

/*
//Waits for data entry via serial
while (Serial.available() > 0)
{
  command = Serial.read();
  
  //Reproduction
  //Stop
  
  if (command == 's') //------------------------STOP
  {
    myDFPlayer.stop();
    Serial.println("Music Stopped!");
    menu_opcoes();
  }
  
  //Pausa/Continua a musica
  if (command == 'p') //------------------------PAUSA
  {
  pausa = !pausa;
  if (pausa == 0)
  {
  Serial.println("Continue...");
  myDFPlayer.start();
  }
  
  if (pausa == 1)
  {
  Serial.println("Music Paused!");
  myDFPlayer.pause();
  }
  
  menu_opcoes();
  }
  
  
  //Increases volume
  if (command == '+')  //---------------------VOL+
  {
    myDFPlayer.volumeUp();
    Serial.print("Current volume:");
    Serial.println(myDFPlayer.readVolume());
    menu_opcoes();
  }
  
  if (command == '<') //---------------------ANTERIOR
  {
    myDFPlayer.previous();
    Serial.println("Previous:");
    Serial.print("Current track:");
    Serial.println(myDFPlayer.readCurrentFileNumber()-1);
    menu_opcoes();
  }
  
  if (command == '>') //---------------------SGTE
  {
    myDFPlayer.next();
    Serial.println("next:");
    Serial.print("Current track:");
    Serial.println(myDFPlayer.readCurrentFileNumber()+1);
    menu_opcoes();
  }
  
  //Decreases volume
  if (command == '-')
  {
    myDFPlayer.volumeDown();
    Serial.print("Current Volume:");
    Serial.println(myDFPlayer.readVolume());
    menu_opcoes();
  }
}
}
void menu_opcoes()
{
  Serial.println();
  Serial.println(F("=================================================================================================================================="));
  Serial.println(F("Commands:"));
  Serial.println(F(" Presenta su tarjeta para reproducir audio"));
  Serial.println(F(" [s] Detiene reproducción"));
  Serial.println(F(" [p] pause/continue music"));
  Serial.println(F(" [+ or -] increases or decreases the volume"));
  Serial.println(F(" [< or >] forwards or backwards the track"));
  Serial.println();
  Serial.println(F("================================================================================================================================="));
} */
