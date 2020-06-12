#include "Wire.h"
#include <Adafruit_PWMServoDriver.h>

// If AD0 pin is set to HIGH set the MPU addr to 0x69
const int MPU = 0x68;
int16_t accelX;
int16_t accelY;

Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
const int minimumPulseWidth = 650;
const int maximumPulseWidth  = 2350;
// PCA9685 board number. Only matters if using more than one.
uint8_t servonum = 0;

int topRedPin = 9;
int bottomRedPin = 12;
int bottomYellowPin = 11;
int topYellowPin = 10;
void setup() {
  
  Serial.begin(9600);
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  // setting modes for pins
  pinMode(topRedPin, INPUT);
  pinMode(topYellowPin, INPUT);
  pinMode(bottomRedPin, INPUT);
  pinMode(bottomYellowPin, INPUT);
  
  // starting PCA9685
  PWMBoard.begin();
  // Servos use 60 Hz LED uses 1.0 KHz
  PWMBoard.setPWMFreq(60);
  
}
// create better function for converting degrees to PWM signals

void loop() {
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  // so far only the accel x and y raw values are being used so only 4 registrars need to be read
  Wire.requestFrom(MPU, 4, true);
  accelX = Wire.read()<<8 | Wire.read();
  accelY = Wire.read()<<8 | Wire.read();
  
  if (accelX >= 3000){
    digitalWrite(bottomYellowPin, HIGH);
    digitalWrite(topRedPin, HIGH);
    }
  else if (accelX <= 0 ){
    digitalWrite(topYellowPin, HIGH);
    digitalWrite(bottomRedPin, HIGH);
    //PWMBoard.setPWM(4, 0, frequencyToAngle(180));
  
  }
  else if (accelY <= -1500){
    digitalWrite(topRedPin, HIGH);
    digitalWrite(bottomRedPin, HIGH);
    
  }
  else if (accelY >= 3000){
    digitalWrite(topYellowPin, HIGH);
    digitalWrite(bottomYellowPin, HIGH);
  }
  else{
  digitalWrite(topRedPin, LOW);
  digitalWrite(topYellowPin, LOW);
  digitalWrite(bottomYellowPin, LOW);
  digitalWrite(bottomRedPin, LOW);
  
  }
  delay(500);
}
