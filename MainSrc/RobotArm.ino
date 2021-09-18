/*  
* Robotic-Arm-Powered-by-Arduino
* roboticArm.ino
* https://github.com/Patchyst
* Created by Patrick Story
*/
#include <Wire.h>
#include <RF24.h> // RF24 driver library; author: TMRh20;Rowberg

#define MPUADDR 0x68

#define CEPIN 7
#define CSPIN 6

/* MPU6050 Registers */
const uint8_t PWR_MGMT_1 = 0x6B;
const uint8_t ACCEL_CONFIG = 0x1c;
enum ACCEL_REGISTERS {ACCEL_XOUT_H=0x3B, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L};
/* MPU6050 Variables */
float ACCEL_XOUT;
float ACCEL_YOUT;
float ACCEL_ZOUT;

/*RF24 variables */
const byte addr[6] = "Recv1";
const int pipe = 0;

RF24 RF24Chip(CEPIN, CSPIN, RF24_SPI_SPEED);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  /* MPU6050 setup */
  Wire.beginTransmission(MPUADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.write(0x00);
 
  while(Wire.endTransmission(true) != 0x00);
  
  
  Wire.beginTransmission(MPUADDR);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x00);
 
  while(Wire.endTransmission(true) != 0x00);
 
  
  /* RF24 setup */
  if(!RF24Chip.begin()){
    Serial.println("[WARNING] RF24 not responding");
  }
  if(!RF24Chip.isChipConnected()){
   Serial.println("[WARNING] RF24 not connected");
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.startListening();
   RF24Chip.openReadingPipe(pipe, addr);

  
  Serial.begin(9600);
}

void loop() {
 Wire.beginTransmission(MPUADDR);
 Wire.write(ACCEL_XOUT_H);
 Wire.endTransmission(false); // Keep connection alive after transmission
 Wire.requestFrom(MPUADDR, 2, true); // request the first 2 registers
 ACCEL_XOUT = (Wire.read()<< 8|Wire.read());
 Serial.println(ACCEL_XOUT);
 Wire.endTransmission();
 if(RF24Chip.available(pipe)){

  }
 

}
