/*  
* Robotic-Arm-Powered-by-Arduino
* roboticArmClient.ino
* https://github.com/Patchyst
* Created by Patrick Story
*/
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <RF24.h> // RF24 driver library; author: TMRh20;Rowberg

// PCA9685
#define SERVO_FREQ 50
// the below constants may need to be further calibrated
#define SERVOMIN 100
#define SERVOMAX 500

// RF24
#define CEPIN 7
#define CSPIN 6

/*RF24 variables */
const byte addr[6] = "Recv1";
const int pipe = 0;

/* variables for data read from the MPU6050 */
int degree;


Adafruit_PWMServoDriver PWMBoard = Adafruit_PWMServoDriver();
RF24 RF24Chip(CEPIN, CSPIN, RF24_SPI_SPEED);

int angleToPWM(int degree){
  if(degree > 180){
   degree = 180;
   }
  else if(degree < 0){
    degree = 0;
  }
  return map(degree, 0, 180, SERVOMIN, SERVOMAX); // mapping degrees to the corresponding PWM signal
}

void setup() {
  Serial.begin(9600);
  /* RF24 setup */
  if(!RF24Chip.begin()){
  }
  if(!RF24Chip.isChipConnected()){
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.startListening();
   RF24Chip.openReadingPipe(pipe, addr);

    /* PCA9685 Setup */
   PWMBoard.setPWMFreq(SERVO_FREQ);
}

void loop() {
 if(RF24Chip.available(pipe)){
    RF24Chip.read(&degree, sizeof(int));
    for(int servo_pin = 0; servo_pin<=12; servo_pin+=4){
      PWMBoard.setPWM(servo_pin, 0, angleToPWM(degree));   
    }
  }
}
