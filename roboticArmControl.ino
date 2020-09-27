//
//  Robotic-Arm-Powered-by-Arduino
//  roboticArmControl.ino
//  https://github.com/Patchyst
//  Created by Patrick Story
//  Copyright © 2020 Patrick Story. All rights reserved.
//

// header for interacting with MPU6050
#include "MPU6050.h"
// header for working with I2C protocl
#include <Wire.h>
// header for interacting with PCA9685
#include <Adafruit_PWMServoDriver.h>

// creating PCA9685 object
Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
// constants for the minimum and maximum PCA9685 pwm value
#define SERVOMIN 100
#define SERVOMAX 500
#define SERVO_FREQ 50

// creating MPU6050 object
MPU6050 mpu;
// If AD0 pin is set to HIGH set the MPU addr to 0x69
const int MPUAddress = 0x68;

// declaring variables for accelerometer and gyroscope as integer with base 16 (hexadecimal)
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;


void setup() {
  
  Serial.begin(9600);
  // waking I2c replica (MPU6050)
  Wire.begin();
  Wire.beginTransmission(MPUAddress);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  // starting PCA9685
  PWMBoard.begin();
  PWMBoard.setOscillatorFrequency(27000000);
  
  // Servos use 60 Hz LED uses 1.0 KHz
  PWMBoard.setPWMFreq(SERVO_FREQ);
  // beginning transmission between I2c board and I2c replica
  Wire.begin(); 
  //Serial.println(" PWM SERVO CHANGE: ")
}
// function for converting degrees to a corresponding value in the corresponding servo range
int angleToServoRange(int degree){
  int pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  return pulseLen;
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

  // PCA9685 pin 4
  PWMBoard.setPWM(4, 0, angleToPWM(abs(angleY)));;
  
  // printing raw X and Y values
  Serial.print("Accel X: ");
  Serial.println(accelX);
  Serial.print("Accel Y: ");
  Serial.println(accelY);
  
  // delay of 200 milliseconds
  delay(200);
}
