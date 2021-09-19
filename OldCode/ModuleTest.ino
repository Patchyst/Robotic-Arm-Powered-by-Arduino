/*  
* Robotic-Arm-Powered-by-Arduino
* https://github.com/Patchyst
* Created by Patrick Story
* Test file made during dev. included for later refrence
*/
#include <Wire.h>
#include <RF24.h> // RF24 driver library; author: TMRh20;Rowberg

#define MPUADDR 0x68

#define CEPIN 7
#define CSPIN 6

struct accel_coordinates{
  float x;
  float y;
  float z;
};

/* MPU6050 Registers */
const uint8_t PWR_MGMT_1 = 0x6B;
const uint8_t ACCEL_CONFIG = 0x1c;
enum ACCEL_REGISTERS {ACCEL_XOUT_H=0x3B, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L};
/* MPU6050 Variables */


const uint8_t config_map = 0b00001000;
float LSB_per_g = 8192;
float error;

/*RF24 variables */
const byte addr[6] = "Recv1";
const int pipe = 0;

RF24 RF24Chip(CEPIN, CSPIN, RF24_SPI_SPEED);

int setup_accelerometer(uint8_t mpu_addr, uint8_t pwr_mgmt, uint8_t accel_config, uint8_t config_bitmap, bool startI2C){
  if(startI2C){
    Wire.begin();
  }
  
  Wire.beginTransmission(mpu_addr);
  Wire.write(pwr_mgmt);
  Wire.write(0x00);
  while(Wire.endTransmission(true) != 0x00);

  Wire.beginTransmission(mpu_addr);
  Wire.write(accel_config);
  Wire.write(config_bitmap);
  while(Wire.endTransmission(true) != 0x00);

  return 0;
}

int read_accel_data(uint8_t mpu_addr, bool end_trans, float lsb_per_g, struct accel_coordinates *raw_xyz){

 Wire.beginTransmission(mpu_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);
 Wire.requestFrom(mpu_addr, 6, true);
  
  raw_xyz->x = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  raw_xyz->y = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  raw_xyz->z = (Wire.read() << 8 | Wire.read()) / lsb_per_g;
  if(end_trans){
    Wire.endTransmission(true);
   }
  return 0;
}


float calibrate_accelerometer(uint8_t mpu_addr, bool instruct, float lsb_per_g, uint32_t limit){
  struct accel_coordinates xyz;
  float error = 1.0;
  float d_step = 0.001;
  
  read_accel_data(mpu_addr, true, lsb_per_g, &xyz);
  
  if(instruct){
    Serial.println("Place MPU6050 on a flat surface and wait for the error to be found");  
  }
  while((xyz.x != 0 && xyz.y != 0) && limit > 0){
      if(xyz.x > 0 or xyz.y > 0){
        error+=d_step; 
       } else if(xyz.x < 0 or xyz.y < 0){ 
        error-=d_step;
       }
      read_accel_data(mpu_addr, true, lsb_per_g, &xyz);
      xyz.x/=error;
      xyz.y/=error;
      if(xyz.x== 0 or xyz.y == 0){
        limit = 0;  
      } else {
        limit-=1;
        }
  }

  return error;
}

void setup() {
  Serial.begin(9600);
  /* MPU6050 setup */
  Serial.println("Setting up accelerometer");
  while(setup_accelerometer(MPUADDR, PWR_MGMT_1, ACCEL_CONFIG, config_map, true) != 0);
  Serial.println("Calibrating accelerometer");
  error = calibrate_accelerometer(MPUADDR, false, LSB_per_g, 0xFFFFFFF);
  Serial.println("Finished calibrating, error found: ");
  Serial.print(error);
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
  int x = 1;
  struct accel_coordinates xyz; 
  read_accel_data(MPUADDR, false, LSB_per_g, &xyz);
//  Serial.print("X: ");
//  Serial.print("Y: ");
  Serial.println(xyz.y);
  
 
 if(RF24Chip.available(pipe)){

  }
 
