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
#define joyTrigA 2 //digital pin 2

//Joystick B
#define joyBx A7
#define joyBy A8
#define joyBz A9
#define joyTrigB 3 //digital pin 3

//LCD
#define lcdSCL A5 // Teensy Pin 4.0 #19 //Brown
#define lcdSDA A6 // Teensy Pin 4.0 #18 //Orange

/*OBJECTS*/
LiquidCrystal_I2C lcd(0x27,16,2);

//Radio Setup
RF24 radio(6, 5); // CE (Blue) - Pin6 Teensy; CSN (Yellow) - Pin5 Teensy
  //SCK(Green) - Pin13; MOSI(Dark Green) - Pin11; MISO(White) - Pin12
const byte addresses[][6] = {"10501","10502"};

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
  //printSticksSerial();
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Printing to Serial");
  //printSticksLcd();
  radio.stopListening();
  data1[1] = analogRead(joyBz);
  //data1[1] = -map(data1[1],0,1023,0,180);
  Serial.println(data1[1]);
  radio.write(&data1[1], sizeof(data1[1]));
}

void readSticks() {
  sticks[0] = analogRead(joyAx);
  sticks[1] = analogRead(joyAy);
  sticks[2] = analogRead(joyAz);
  sticks[3] = analogRead(joyTrigA);
  sticks[4] = analogRead(joyBx);
  sticks[5] = analogRead(joyBx);
  sticks[6] = analogRead(joyBx);
  sticks[7] = analogRead(joyTrigB);
}

void initPins(){
  pinMode(joyAx, INPUT);
  pinMode(joyAy, INPUT);
  pinMode(joyAz, INPUT);
  pinMode(joyTrigA, INPUT);
  pinMode(joyBx, INPUT);
  pinMode(joyBy, INPUT);
  pinMode(joyBz, INPUT);
  pinMode(joyTrigB, INPUT);
  pinMode(lcdSCL, OUTPUT);
  pinMode(lcdSDA, OUTPUT);
}
