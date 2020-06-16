// header for interacting with MPU6050
#include "MPU6050.h"
// header for working with I2C protocl
#include <Wire.h>
// header for interacting with PCA9685
#include <Adafruit_PWMServoDriver.h>

// creating PCA9685 object
Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
// constants for the minimum and maximum PCA9685 pwm value
#define SERVOMIN 650
#define SERVOMAX 2350

// creating MPU6050 object
MPU6050 mpu;
// If AD0 pin is set to HIGH set the MPU addr to 0x69
const int MPUAddress = 0x68;

// declaring variables for accelerometer and gyroscope as integer with base 16 (hexadecimal)
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;



// PCA9685 board number, only matters if using more than one
uint8_t servonum = 0;

void setup() {
  
  Serial.begin(9600);
  // waking I2c slave (MPU6050)
  Wire.begin();
  Wire.beginTransmission(MPUAddress);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  // starting PCA9685
  PWMBoard.begin();
  // Servos use 60 Hz LED uses 1.0 KHz
  PWMBoard.setPWMFreq(60);
  // beginning transmission between I2c board and I2c slave
  Wire.begin(); 
  //Serial.println(" PWM SERVO CHANGE: ")
}
// function for converting degrees to a corresponding frequency value that the PCA9685 can understand
int angleToFreq(int degree){
  int pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  int returnAngle = int(float(pulseLen) / 1000000 * 204800);
  return returnAngle;
  }

void loop() {
  // reading raw values from MPU6050
  mpu.getMotion6 (&accelX, &accelY, &accelZ, &gyroZ, &gyroY, &gyroZ);
  // using map to convert raw X and Y values from accelerometer into degrees 0-180
  int angleX = map (accelX / 131 , -17000 / 131, 17000 / 131, 0, 180);
  int angleY = map (accelY / 131 , -17000 / 131, 17000 / 131, 0, 180);
  
  // taking the absolute value to ensure no negative numbers are given
  Serial.println (abs(angleX));
  Serial.println (abs(angleY));

  // PCA9685
  PWMBoard.setPWM(4, 0, angleToFreq(abs(angleY)));;
  
  // printing raw X and Y values
  Serial.print("Accel X: ");
  Serial.println(accelX);
  Serial.print("Accel Y: ");
  Serial.println(accelY);
  
  // delay of 200 milliseconds
  delay(500);
}
