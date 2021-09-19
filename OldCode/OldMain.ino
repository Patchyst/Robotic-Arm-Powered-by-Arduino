

#include "MPU6050.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
// constants for the minimum and maximum PCA9685 pwm value
#define SERVOMIN 100
#define SERVOMAX 500
#define SERVO_FREQ 50

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
  if(degree > 180){
    for(int i; degree>180; i++){
      degree -= 1;
      }
    }
   else if(degree < 0){
    degree = 0;
    }
  int pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  return pulseLen;
}


void loop() {
  // reading raw values from MPU6050
  mpu.getMotion6 (&accelX, &accelY, &accelZ, &gyroZ, &gyroY, &gyroZ);
  // using map to convert raw X and Y values from accelerometer into degrees 0-180
  int angleX = map (accelX / 131 , -17000 / 131, 17000 / 131, 0, 180);
  int angleY = map (accelY / 131 , -17000 / 131, 17000 / 131, 0, 180);

  // PCA9685 pin 0
  PWMBoard.setPWM(0, 0, angleToServoRange(angleY));
  
  // printing raw X and Y values
  Serial.print("Accel X: ");
  Serial.println(accelX);
  Serial.print("Accel Y: ");
  Serial.println(accelY);
  
  // delay of 200 milliseconds
  delay(200);
}
