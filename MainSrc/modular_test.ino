/*  
* Robotic-Arm-Powered-by-Arduino
* roboticArm.ino
* https://github.com/Patchyst
* Created by Patrick Story
*/
#include <stdlib.h>
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

int setup_accelerometer(uint8_t mpu_addr, uint8_t pwr_mgmt, uint8_t accel_config, uint16_t config_bitmap){
  Wire.beginTransmission(mpu_addr);
  Wire.write(pwr_mgmt);
  Wire.write((uint8_t)(config_bitmap & 0xff00));
  while(Wire.endTransmission(true) != 0x00);

  Wire.beginTransmission(mpu_addr);
  Wire.write(accel_config);
  Wire.write((uint8_t)((config_bitmap & 0x00ff)<<8));
  while(Wire.endTransmission(true) != 0x00)

  return EXIT_SUCCESS
}

int read_accel_data(uint8_t mpu_addr, bool end_trans){
  float raw_xyz[3];

 Wire.beginTransmission(mpu_addr);
 Wire.write(0x3B);
 Wire.endTransmission(end_trans)
 Wire.requestFrom(mpu_addr, 6, true);
  
  raw_xyz[0] = Wire.read() << 8 | Wire.read();
  raw_xyz[1] = Wire.read() << 8 | Wire.read();
  raw_xyz[2] = Wire.read() << 8 | Wire.read();

  return raw_xyz 
}


int calibrate_accelerometer(uint8_t mpu_addr, bool instruct){
  float error = 0.0;
  if(instruct){
    Serial.println("Place MPU6050 on a flat surface and wait for the error to be found");  
  }
  while(read_accel_data(mpu_addr, true)[0] > 0 && read_accel_data(mpu_addr, true)[1] > 0){
      
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
 
  
  /* RF24 setup */
  if(!RF24Chip.begin()){
  }
  if(!RF24Chip.isChipConnected()){
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.startListening();
   RF24Chip.openReadingPipe(pipe, addr);
}

void loop() {
 Wire.beginTransmission(MPUADDR);
 Wire.write(ACCEL_XOUT_H);
 Wire.endTransmission(false);
 Wire.requestFrom(MPUADDR, 2, true);
 ACCEL_XOUT = (Wire.read()<< 8|Wire.read());
 Serial.println(ACCEL_XOUT);
 Wire.endTransmission();
 if(RF24Chip.available(pipe)){

  }
 

}
