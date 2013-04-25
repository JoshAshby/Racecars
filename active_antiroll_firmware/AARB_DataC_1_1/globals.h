//All the variable declaration happens here to clean up the main file

// Declaring an union for the registers and the axis values.
// The byte order does not match the byte order of 
// the compiler and AVR chip.
// The AVR chip (on the Arduino board) has the Low Byte 
// at the lower address.
// But the MPU-6050 has a different order: High Byte at
// lower address, so that has to be corrected.
// The register part "reg" is only used internally, 
// and are swapped in code.
#ifndef GLOBALS_H
#define GLOBALS_H
#include "card.h"

int wheel = 0;
int selector = A0;
int speedsensorfl = A2;
int speedsensorfr = A3;
int speedsensorrl = 5;
int speedsensorrr = A1;

int recswitch = 3;
int recled = 4;

int bonnieandclyde = 7;
int mustang = 9;
int sally = 10;
Servo frontLeftServo;
Servo frontRightServo;
Servo rearServo;

int rearslide;
int wheelspeedfl;
int wheelspeedfr;
int wheelspeedrl;
int wheelspeedrr;

int pulsewidthfl;
int pulsewidthfr;
int pulsewidthrl;
int pulsewidthrr;

float ax = 0;
float ay = 0;
float gz = 0;

int setting;
float reading;
boolean recording;
float steeringangle;
unsigned long time;

int error;
uint8_t c;

double dT;
accel_t_gyro_union accel_t_gyro;
speeds_struct wheelSpeeds;
servos_struct servos;

const int chipSelect = 8;

uint8_t swap;
#define SWAP(x,y) swap = x; x = y; y = swap

#if logging
  Log Logger;
#endif

int wheelTimeout = 13900;
int wheelDivisor = 34807;

#endif
