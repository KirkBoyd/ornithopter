/* Flappy Bird!
* Teensy 4.0 Side [on the birdo]       
* Kirk Boyd, kirkboyd.xyz; May 1, 2021
* Adapted from parts of examples by Dejan Nedelkovski, www.HowToMechatronics.com
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

/* Libraries */
#include <SPI.h> //wireless interface protocol
#include <nRF24L01.h> //wireless receiver
#include <RF24.h> //wireless receiver
#include <PWMServo.h> //control the ESC with PWM signals like a servo motor (i.e. 0 = stopped, 180 = max speed)
//#include <Servo.h> //control the tail servo
#include <Wire.h>
#include <Adafruit_Sensor.h> //accelerometer
#include <Adafruit_BNO055.h> //accelerometer
#include <utility/imumaths.h> //accelerometer calculations

/* Constants */
#define BNO055_sampleRate 10//delay in ms of samples

/* Pins/Ports */
#define escPin 2 //digital pin 2 for pwm signal
#define hallPin1 A9 //analog port 9 (pin 23 on Teensy)
#define hallPin2 A8 //analog port 8 (pin 22 on Teensy)
#define latchLimSw 4 //digital pin 4 for limit switch which verifies if latch is open or closed
#define latchTrig 14 //digital pin 14 for Power N-FET which powers 12v through solenoid
#define tailServoPin 8 //digital pin 8 for servo pin which controls tail feather attitude

/* Radio Setup*/
RF24 radio(6, 5); // CE, CSN
const byte addresses[][6] = {"10501","10502"};

struct ROBOMOTE_DATA_STRUCTURE{ //data received from the remote and stored here. Must match "SEND_" on other arduino
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
ROBOMOTE_DATA_STRUCTURE data_robomote; //create an object of this type

struct BIRDO_DATA_STRUCTURE{
  int16_t accelX;
  int16_t accelY;
  int16_t hall1;
  int16_t hall2;
};

int accelX;
int accelY;
int hall1;
int hall2;
BIRDO_DATA_STRUCTURE data_birdo; //create an object of this type


/* ESC Setup */
PWMServo ESC;
PWMServo Tail;
const int pwmMin = 1000;
const int pwmMax = 2000;

/* Accelerometer Setup */
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

/* Variables */
int speedESC = 0;

void setup() {
  pinMode(hallPin1,INPUT);
  pinMode(hallPin2,INPUT);
  ESC.attach(escPin,pwmMin,pwmMax);
  Tail.attach(tailServoPin);
  Tail.write(90);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
  if(!bno.begin()){
    Serial.println("No BNO055 detected.");
    while(1);
  }
  bno.setExtCrystalUse(true);
  Serial.begin(115200);
}

void loop() {
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  
  radio.startListening();
  if(radio.available()){ //check if there is a signal coming in
      radio.read(&data_robomote,sizeof(ROBOMOTE_DATA_STRUCTURE)); //recieve potentiometer value

      //ESC.write(pot1); //write potentiometer val to esc
    //radio.stopListening();
  }
  else if(millis()%4 == 0){
    radio.stopListening();
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    data_birdo.hall1 = analogRead(hallPin1);
    data_birdo.hall2 = analogRead(hallPin2);
    data_birdo.accelX = accel.x()*1000;
    data_birdo.accelY = accel.y()*1000;
    radio.write(&data_birdo,sizeof(BIRDO_DATA_STRUCTURE));
    printData();
    radio.startListening();
  }
  //else{ 
  //}
  

  //printSticksSerial();

  if (data_robomote.Ay<10 && data_robomote.By<25 && data_robomote.Atrig>0 && data_robomote.Btrig >0){
    speedESC = 0;
  }
  if (data_robomote.Ay>1000 && data_robomote.By>1000 && data_robomote.Atrig>0 && data_robomote.Btrig >0){
    speedESC = 180; // set wings to max speed
    digitalWrite(latchTrig,HIGH);
    delay(5);
    digitalWrite(latchTrig,LOW);
  }
  if (data_robomote.Az>1000 && data_robomote.Atrig>0 && data_robomote.Btrig>0){
    speedESC++;
    //ESC.write(speedESC);
  }
  //printData();
  ESC.write(speedESC);
}

void readBirdo(){

}
