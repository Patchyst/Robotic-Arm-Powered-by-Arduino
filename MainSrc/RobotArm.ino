/*
 * 
 * 
 * 
 */
#include <Wire.h>
#include <RF24.h> // RF24 driver library; author: TMRh20;Rowberg

#define MPUADDR 0x68

#define CEPIN 7
#define CSPIN 6

/* MPU6050 Registers */
const uint8_t PWR_MGMT_1 = 0x6B;
enum ACCEL_REGISTERS {ACCEL_XOUT1=0x3B, ACCEL_XOUT2, ACCEL_YOUT1, ACCEL_YOUT2, ACCEL_ZOUT1, ACCEL_ZOUT};

/*RF24 variables */
const byte addr[6] = "Recv1";
const int pipe = 0;

RF24 RF24Chip(CEPIN, CSPIN, RF24_SPI_SPEED);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  /* MPU6050 setup */
  Wire.beginTransmission(MPUADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0);
  Wire.write(0x80); // Setting DEVICE_RESET bit
  Wire.endTransmission();
  /* RF24 setup */
  if(!RF24Chip.begin()){
    Serial.println("[WARNING] RF24 not responding");
  }
  if(!RF24Chip.isChipConnected()){
   Serial.println("[WARNING] RF24 not connected");
   }
   RF24Chip.setPALevel(RF24_PA_MIN);
   RF24Chip.startListening();
   RF24Chip.openReadingPipe(pipe, addr);

  Serial.println(ACCEL_XOUT2);
}

void loop() {
 if(RF24Chip.available(pipe)){

  }
 

}
