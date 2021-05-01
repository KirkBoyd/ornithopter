/**************************************************
 * Robomote - Flappy Bird Configuration
 * Teensy 4.0  for faster transfer (hopefully)
 * Kirk Boyd
 * 04/29/2021
 *************************************************/
 /*LIBRARIES*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //best functioning I2C library I could find which works with Teensy and the LCD I had lying around
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/*VARIABLES*/
//Joystick A
#define joyAx A2
#define joyAy A1
#define joyAz A3
#define joyTrigA 3 //digital pin 3

//Joystick B
#define joyBx A7
#define joyBy A8
#define joyBz A9
#define joyTrigB 2 //digital pin 2

//LCD
#define lcdSCL A5 // Teensy Pin 4.0 #19 //Brown
#define lcdSDA A6 // Teensy Pin 4.0 #18 //Orange

/*OBJECTS*/
LiquidCrystal_I2C lcd(0x27,16,2);

//Radio Setup
RF24 radio(6, 5); // CE (Blue) - Pin6 Teensy; CSN (Yellow) - Pin5 Teensy
  //SCK(Green) - Pin13; MOSI(Dark Green) - Pin11; MISO(White) - Pin12
const byte addresses[][6] = {"10501","10502"};

struct ROBOMOTE_DATA_STRUCTURE{ //data sent from this device, must match receive on other arduino
  int16_t Ax;
  int16_t Ay;
  int16_t Az;
  int16_t Atrig;
  int16_t Bx;
  int16_t By;
  int16_t Bz;
  int16_t Btrig;
};
  int Ax;
  int Ay;
  int Az;
  int Atrig;
  int Bx;
  int By;
  int Bz;
  int Btrig;
ROBOMOTE_DATA_STRUCTURE data_robomote; //create an object of send structure

struct BIRDO_DATA_STRUCTURE{
  int16_t accelX;
  int16_t accelY;
  int16_t hall1;
  int16_t hall2;
};


BIRDO_DATA_STRUCTURE data_birdo; //create an object of receive structure

/* Dynamic Variables */
int pot1 = 0;
int data1[3];
int sticks[8];

void setup() {
  initPins();
  /* Radio */
  radio.begin();
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  /* LCD and Serial */
  lcd.init(); //initialize LCD screen
  lcd.init(); //initialize again? (Idk this was in the example)
  lcd.backlight(); //activate backlight on the screen
  lcd.print("Robomote Init");
  Serial.begin(115200);
  Serial.println("Robomote Init");
}

void loop() {
  readSticks();
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Printing to Serial");
  radio.stopListening();
  if (millis()%4 == 0){printSticksLcd();}
  radio.write(&data_robomote, sizeof(ROBOMOTE_DATA_STRUCTURE));
  radio.startListening();
  delay(4);
  if (radio.available()){
    
    radio.read(&data_birdo, sizeof(BIRDO_DATA_STRUCTURE));
    printBirdo ();
    radio.stopListening();
    
  }
}

void readSticks() {
  sticks[0] = (1023- analogRead(joyAx) );
  sticks[1] =   analogRead(joyAy);
  sticks[2] =   analogRead(joyAz);
  sticks[3] =   digitalRead(joyTrigA);
  sticks[4] =   analogRead(joyBx);
  sticks[5] = (1023- analogRead(joyBy) );
  sticks[6] =   analogRead(joyBz);
  sticks[7] =   digitalRead(joyTrigB);

  data_robomote.Ax    =   sticks[0];
  data_robomote.Ay    =   sticks[1];
  data_robomote.Az    =   sticks[2];
  data_robomote.Atrig =   sticks[3];
  data_robomote.Bx    =   sticks[4];
  data_robomote.By    =   sticks[5];
  data_robomote.Bz    =   sticks[6];
  data_robomote.Btrig =   sticks[7];
}

void initPins(){
  pinMode(joyAx,    INPUT);
  pinMode(joyAy,    INPUT);
  pinMode(joyAz,    INPUT);
  pinMode(joyTrigA, INPUT);
  pinMode(joyBx,    INPUT);
  pinMode(joyBy,    INPUT);
  pinMode(joyBz,    INPUT);
  pinMode(joyTrigB, INPUT);
  pinMode(lcdSCL,   OUTPUT);
  pinMode(lcdSDA,   OUTPUT);
}
