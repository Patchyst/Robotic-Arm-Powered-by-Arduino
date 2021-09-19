# Controlling Robotic Arm with hands.
5 MPU6050 modules are attached to the fingers of a glove. The raw accelerometer values are read from the registers and converted to a corresponding servo degree (0°-180°) using the iverse tangeant of the acceleration of gravity (-1g to 1g) on the Z and Y axis.
