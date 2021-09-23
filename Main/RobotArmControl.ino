/*  
* Robotic-Arm-Powered-by-Arduino
* RobotArmControl.ino
* https://github.com/Patchyst
* Created by Patrick Story
*/
#include <Wire.h>
#include <RF24.h> 


// MPU6050
#define MPUADDR 0x68
//RF24
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
float degree = 1;
int rounded_degree = 0;

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


float fix_error(float val, float max_v, float low_v){
  if(val>max_v){
    return val-(val-max_v);
   } else if(val<low_v){
    return val-(val+abs(low_v));
   } else {
    return val;
    }
}

int get_quadrant(float x_pos, float y_pos){
  if(x_pos == abs(x_pos) and y_pos == abs(y_pos)){
    return 1;
   } 
   else if(x_pos != abs(x_pos) and y_pos == abs(y_pos)) {
    return 2;   
   }
   else if(x_pos == abs(x_pos) and y_pos != abs(y_pos)) {
    return 4;
   } else {
      return 3;
    }
  
 }

void setup() {
  Serial.begin(9600);
  /* MPU6050 setup */
  while(setup_accelerometer(MPUADDR, PWR_MGMT_1, ACCEL_CONFIG, config_map, true) != 0);
  /* RF24 setup */
  if(!RF24Chip.begin()){
    Serial.println("[FATAL] RF24 not responding");
  }
  if(!RF24Chip.isChipConnected()){
   Serial.println("[FATAL] RF24 not connected");
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.stopListening();
   RF24Chip.openWritingPipe(addr);
   Serial.println("Finished setting up ");
}

void loop() {
    /* Because the Z axis reads 1 when the MPU6050 is perfectly horizontal, it will be the new x axis.
  This allows the use of basic trig to find the angle of orientation based on quadrant */
  
  struct accel_coordinates xyz;
  read_accel_data(MPUADDR, false, LSB_per_g, &xyz);
  
  xyz.z = fix_error(xyz.z, 1, -1);
  xyz.y = fix_error(xyz.y, 1, -1);
  /* Handles the edge case of g < -1 or g > 1. Usually due to minor, external forces disrupting the reading */
  if(abs(xyz.z) == 1){
    xyz.y = 0;
    degree = 0;
  }
  else if(abs(xyz.y) == 1){
    xyz.z = 0;
    degree = 90*xyz.y;
   } 
   else {
    degree = atan(xyz.y/xyz.z)*RAD_TO_DEG;
   }
  /* Note that the following switch statement changes degrees to specifically fit this project */
  switch (get_quadrant(xyz.z, xyz.y)){ 
    case 1: // fingers should not move anti-clockwise from the +x
      degree = 0;
      break;
    case 4:
      degree = abs(degree);
      break;
    case 3:
      degree = (90-degree)+90; // make the degree relative to the -y then add 90
      break;
    case 2: // fingers should not move clockwise from the -x
      degree = 180; 
      break;
  }
 rounded_degree = (int)(degree+0.5);
 Serial.println(rounded_degree);
 RF24Chip.write(&rounded_degree, sizeof(int));
 

}
