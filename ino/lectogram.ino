#include <SPI.h>
#include <MFRC522.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

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
  while (true);
}
Serial.println();
Serial.println(F("DFPlayer Mini module initialized!"));
myDFPlayer.setTimeOut(500); //Timeout serial 500ms
myDFPlayer.volume(25); //Volume máx 30
myDFPlayer.EQ(0); //Equalizacao normal
//menu_opcoes();

SPI.begin();        //Iniciamos el Bus SPI
mfrc522.PCD_Init(); // Iniciamos el MFRC522
Serial.println("Control de acceso:");
command='6';                                    //bienvenido al sistema
command = command - 48;
myDFPlayer.play(command);

for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, celeste); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   } 

for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, verde); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   } 
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0xB9, 0x99, 0x45, 0xE8} ; //código del usuario 1
byte Usuario2[4]= {0x6C, 0x38, 0x5A, 0x89} ; //código del usuario 2
byte Usuario3[4]= {0x0D, 0x8E, 0x59, 0x89} ; //código del usuario 3
byte Usuario4[4]= {0xBA, 0xFF, 0xCC, 0x73} ; //código del usuario 4
byte Usuario5[4]= {0xF2, 0x76, 0x5A, 0x89} ; //código del usuario 5

/*
 * TARJETAS
1-> B9 99 45 E8
2-> 6C 38 5A 89
3-> 0D 8E 59 89
4-> BA FF CC 73
5-> F2 76 5A 89


0001_bienvenido
0002_living
0003_baño
0004_cocina
0005_balcon
0006_pieza
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
                  if(compareArray(ActualUID,Usuario1)) //------------------------------------------usuario 1
                    {
                   // Serial.println("Acceso concedido...");
                    command='1';
                    LED_ROJO();
                    }
                  else if(compareArray(ActualUID,Usuario2)) //-------------------------------------usuario 2
                    {
                   // Serial.println("Acceso concedido...");
                    command='2';  
                    LED_AZUL();                  
                    }
                  else if(compareArray(ActualUID,Usuario3)) //-------------------------------------usuario 2
                    {
                   // Serial.println("Acceso concedido...");
                    command='3';
                    LED_AMARILLO();                    
                    }
                   else if(compareArray(ActualUID,Usuario4)) //-------------------------------------usuario 2
                    {
                   // Serial.println("Acceso concedido...");
                    command='4';
                    LED_VERDE();                    
                    }
                   else if(compareArray(ActualUID,Usuario5)) //-------------------------------------usuario 2
                    {
                   // Serial.println("Acceso concedido...");
                    command='5'; 
                    LED_CELESTE();                  
                    }
                  else
                   { Serial.println("Acceso denegado...");}
                                   
                  mfrc522.PICC_HaltA(); // Terminamos la lectura de la tarjeta tarjeta  actual
                  if ((command >= '1') && (command <= '9'))
                    {
                    Serial.print("Music reproduction");
                    Serial.println(command);
                    command = command - 48;
                    myDFPlayer.play(command);
                   // menu_opcoes();
                    }
            }            
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

void LED_ROJO()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, rojo); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_VERDE()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i,verde); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}

void LED_AZUL()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, azul); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_AMARILLO()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, amarillo); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_MORADO()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i,morado); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
   }  
}
void LED_CELESTE()
{
  for(int i=0;i<NUMPIXELS;i++)
   {
    pixels.setPixelColor(i, celeste); // Brillo moderado en rojo
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
    delay(delayval); // Pausa por un periodo de tiempo (en milisegundos).
    pixels.setPixelColor(i, apagado); // Brillo moderado en azul
    pixels.show();   // Mostramos y actualizamos el color del pixel de nuestra cinta led RGB
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
