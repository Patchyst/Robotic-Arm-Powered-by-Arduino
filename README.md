# Script for Controlling Robot Arm with hands.
5 MPU6050 modules are attached to the fingers of a glove. The raw accelerometer X and Y values are read from the registers and converted to a corresponding servo degree (0°-180°) using the map() function (lines 56-57). These values are sent to servos controlled by the PCA9685 module allowing the robotic arm to match the user's finger movements.
