#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVOMIN  100
#define SERVOMAX  500
#define SERVO_FREQ 50
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
}

int angleToServoRange(int degree){
  int pulseLen = map(degree, 0, 180, SERVOMIN, SERVOMAX);
  return pulseLen;
}

void loop() {
  pwm.setPWM(servonum, 0, angleToServoRange(0));
  delay(1000);
  pwm.setPWM(servonum, 0, angleToServoRange(180));
  delay(1000);
}
