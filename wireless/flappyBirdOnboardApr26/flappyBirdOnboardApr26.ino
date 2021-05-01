/* Flappy Bird!
* Teensy 4.0 Side [on the birdo]       
* Kirk Boyd, kirkboyd.xyz; Apr 18 , 2021
* Adapted from parts of examples by Dejan Nedelkovski, www.HowToMechatronics.com
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

/* Libraries */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PWMServo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Constants */
#define BNO055_sampleRate //delay in ms of samples

/* Pins/Ports */
#define escPin 2 //digital pin 2 for pwm signal
#define hallPin1 A9 //analog port 9 (pin 23 on Teensy)
#define hallPin2 A8 //analog port 8 (pin 22 on Teensy)

/* Radio Setup*/
RF24 radio(6, 5); // CE, CSN
const byte addresses[][6] = {"10501","10502"};

/* ESC Setup */
PWMServo ESC;
const int pwmMin = 1000;
const int pwmMax = 2000;

/* Accelerometer Setup */
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);

/* Variables */
int pot1 = 0;
float accelData[2];
long allData[5];
long yAccel;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
  //radio.stopListening();
  pinMode(hallPin1,INPUT);
  pinMode(hallPin2,INPUT); 
  //pinMode(escPin,OUTPUT); //we will write PWM signals to control the motor speed
  ESC.attach(escPin,pwmMin,pwmMax);
  if(!bno.begin()){
    Serial.println("No BNO055 detected.");
    while(1);
  }
  bno.setExtCrystalUse(true);
}

void loop() {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  radio.startListening();
  delay(3);
  if(radio.available()){ //check if there is a signal coming in
    //while(radio.available()){
      radio.read(&pot1,sizeof(pot1)); //recieve potentiometer value
      Serial.print("Pot: ");
      Serial.println(pot1);
      //Serial.print("\t");
      ESC.write(pot1); //write potentiometer val to esc
    //}
    radio.stopListening();
  }
  else{ 
    //Serial.print("\t\t");
    //radio.stopListening();
  }
  
  radio.stopListening();
  int data1[] = {analogRead(hallPin1)}; //,analogRead(hallPin2)};
  accelData[0] = accel.x();
  accelData[1] = accel.y();
  Serial.print(data1[0]);
  Serial.print(" ");
  //Serial.print(data1[1]);
  //Serial.print(" ");
  radio.write(&data1[0], sizeof(data1[0]));
  //delay(5);
  //radio.write(&data1[1], sizeof(data1[1]));
  delay(20);
  radio.write(&accelData[0],sizeof(accelData[0]));
  //delay(5);
  radio.write(&accelData[1],sizeof(accelData[1]));
  //delay(5);
  Serial.println();
}
