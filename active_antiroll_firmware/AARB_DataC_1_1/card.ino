/*
Joshua Ashby 2013
joshuaashby@joshashby.com
Logger object

Overly simplified logger object to make the normal code
cleaner by removing all the log aspects. Also makes the code
easier to work with and modify.

Designed and built with no love for the
2013 CSU FSAE Active Antiroll System by Mike Goen
*/
#include "card.h"
 
//constructor
Log::Log(){
}
 
//destructor
Log::~Log(){
}

void Log::begin() {
  #if debug
    //Start up serial, so we can constantly log to it.
    Serial.begin(9600);
  #endif
}

void Log::open() {
  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    #if debug
      Serial.println("Failed to connect to card, skipping logging to microSD...");
    #endif
  }

  // open the file for write at end like the Native SD library
  if (!logFile.open("log.csv", O_RDWR | O_CREAT | O_AT_END)) {
    #if debug
      Serial.println("Failed to open log.csv, skipping microSD logging...");
    #endif 
  } else {
    // if the file opened okay, write to it:
    #if debug
      Serial.println("Logging to log.csv...");
    #endif
    #if full
      logFile.println("timems,xAccel,yAccel,zAccel,temp,xGyro,yGyro,zGyro,frontRightSpeed,frontLeftSpeed,rearRightSpeed,rearLeftSpeed,frontRightServo,frontLeftServo,rearServo,message");
    #else
      logFile.println("timems,xAccel,yAccel,zGyro,frontRightSpeed,frontLeftSpeed,rearRightSpeed,rearLeftSpeed,frontRightServo,frontLeftServo,rearServo");
    #endif
  }
}

void Log::close() {
  // close the file:
  logFile.close();
}

void Log::beginLog() {
  unsigned long time = millis();
  logFile.print(time);
  logFile.print(",");

  #if debug
    Serial.print(time);
    Serial.print(",");
  #endif
}

void Log::error(String message) {
  logFile.println("**Begin Error:: " + message + " ::End Error**");

  #if debug
    Serial.println("**Begin Error:: " + message + " ::End Error **");
  #endif
}

void Log::log(String message) {
  logFile.println(message);

  #if debug
    Serial.println(message);
  #endif
}

#if full
  void Log::log(String message, const accel_t_gyro_union& aAndG, const speeds_struct& speeds, const servos_struct&  servos) {
    beginLog();
    logFile.print(aAndG.value.x_accel, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.y_accel, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.z_accel, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.x_gyro, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.y_gyro, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.z_gyro, DEC);
    logFile.print(",");
    logFile.print(speeds.frontRight, DEC);
    logFile.print(",");
    logFile.print(speeds.frontLeft, DEC);
    logFile.print(",");
    logFile.print(speeds.rearRight, DEC);
    logFile.print(",");
    logFile.print(speeds.rearLeft, DEC);
    logFile.print(",");
    logFile.print(servos.frontRight, DEC);
    logFile.print(",");
    logFile.print(servos.frontLeft, DEC);
    logFile.print(",");
    logFile.print(servos.rear, DEC);
    logFile.print(",");
    logFile.print(message, DEC);
    logFile.println();
    #if debug
      Serial.print(aAndG.value.x_accel, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.y_accel, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.z_accel, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.x_gyro, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.y_gyro, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.z_gyro, DEC);
      Serial.print(",");
      Serial.print(speeds.frontRight, DEC);
      Serial.print(",");
      Serial.print(speeds.frontLeft, DEC);
      Serial.print(",");
      Serial.print(speeds.rearRight, DEC);
      Serial.print(",");
      Serial.print(speeds.rearLeft, DEC);
      Serial.print(",");
      Serial.print(servos.frontRight, DEC);
      Serial.print(",");
      Serial.print(servos.frontLeft, DEC);
      Serial.print(",");
      Serial.print(servos.rear, DEC);
      Serial.print(",");
      Serial.print(message);
      Serial.println();
    #endif
  }
#else
  void Log::log(const accel_t_gyro_union& aAndG, const speeds_struct& speeds, const servos_struct&  servos) {
    beginLog();
    logFile.print(aAndG.value.x_accel, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.y_accel, DEC);
    logFile.print(",");
    logFile.print(aAndG.value.z_gyro, DEC);
    logFile.print(",");
    logFile.print(speeds.frontRight, DEC);
    logFile.print(",");
    logFile.print(speeds.frontLeft, DEC);
    logFile.print(",");
    logFile.print(speeds.rearRight, DEC);
    logFile.print(",");
    logFile.print(speeds.rearLeft, DEC);
    logFile.print(",");
    logFile.print(servos.frontRight, DEC);
    logFile.print(",");
    logFile.print(servos.frontLeft, DEC);
    logFile.print(",");
    logFile.print(servos.rear, DEC);
    logFile.println();
    #if debug
      Serial.print(aAndG.value.x_accel, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.y_accel, DEC);
      Serial.print(",");
      Serial.print(aAndG.value.z_gyro, DEC);
      Serial.print(",");
      Serial.print(speeds.frontRight, DEC);
      Serial.print(",");
      Serial.print(speeds.frontLeft, DEC);
      Serial.print(",");
      Serial.print(speeds.rearRight, DEC);
      Serial.print(",");
      Serial.print(speeds.rearLeft, DEC);
      Serial.print(",");
      Serial.print(servos.frontRight, DEC);
      Serial.print(",");
      Serial.print(servos.frontLeft, DEC);
      Serial.print(",");
      Serial.print(servos.rear, DEC);
      Serial.println();
    #endif
  }
#endif
