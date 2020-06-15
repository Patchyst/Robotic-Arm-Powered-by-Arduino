#include "MPU6050.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// If AD0 pin is set to HIGH set the MPU addr to 0x69
const int MPUAddress = 0x68;
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;
Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
MPU6050 mpu;
const int minimumPulseWidth = 650;
const int maximumPulseWidth  = 2350;
uint8_t servonum = 0;
void setup() {
  
  Serial.begin(9600);
  
  Wire.begin();
  Wire.beginTransmission(MPUAddress);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  // starting PCA9685
  PWMBoard.begin();
  // Servos use 60 Hz LED uses 1.0 KHz
  PWMBoard.setPWMFreq(60);
  Wire.begin();
  
  
}

void loop() {
  
  mpu.getMotion6 (&accelX, &accelY, &accelZ, &gyroZ, &gyroY, &gyroZ);
  int angleX = map (accelX / 131 , -17000 / 131, 17000 / 131, 0, 180) ;

  Serial.println (angleX);
  Serial.print("Accel X: ");
  Serial.println(accelX);
  Serial.print("Accel Y: ");
  Serial.println(accelY);
  
  delay(500);
}
