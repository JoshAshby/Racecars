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
      logFile.println("time,xAccel,yAccel,zAccel,temp,xGyro,yGyro,zGyro,frontRightSpeed,frontLeftSpeed,rearRightSpeed,rearLeftEpeed,frontRightServo,frontLeftServo,rearServo,message");
    #else
      logFile.println("time,xAccel,yAccel,zGyro,frontRightSpeed,frontLeftSpeed,rearRightSpeed,rearLeftEpeed,frontRightServo,frontLeftServo,rearServo");
    #endif
  }
}

void Log::close() {
  // close the file:
  logFile.close();
}

#if full
  void Log::log(String message) {
    logFile.println("**Begin Error:: " + message + " ::End Error**");

    #if debug
      Serial.println("**Begin Error:: " + message + " ::End Error **");
    #endif
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

  void Log::log(String message, int *aAndGBuffer) {
    logAccel((int *)&aAndGBuffer);
    logFile.println(message);

    #if debug
      Serial.println(message);
    #endif
  }

  void Log::log(String message, int *aAndGBuffer, float *wSpeedBuffer) {
    beginLog();
    logAccel((int *)&aAndGBuffer);
    logSpeed((float *)&wSpeedBuffer);
    logFile.println(message);

    #if debug
      Serial.println(message);
    #endif
  }

  void Log::log(String message, int *aAndGBuffer, float *wSpeedBuffer, int *pulseBuffer) {
    beginLog();
    logAccel((int *)&aAndGBuffer);
    logSpeed((float *)&wSpeedBuffer);
    logPulse((int *)&pulseBuffer);
    logFile.println(message);

    #if debug
      Serial.println(message);
    #endif
  }
#else
  void Log::beginLog() {
    unsigned long time = millis();
    logFile.print(time);
    logFile.print(",");

    #if debug
      Serial.print(time);
      Serial.print(",");
    #endif
  }

  void Log::log(int *aAndGBuffer) {
    logAccel((int *)&aAndGBuffer);
    logFile.println();

    #if debug
      Serial.println();
    #endif
  }

  void Log::log(int *aAndGBuffer, float *wSpeedBuffer) {
    beginLog();
    logAccel((int *)&aAndGBuffer);
    logSpeed((float *)&wSpeedBuffer);
    logFile.println();

    #if debug
      Serial.println();
    #endif
  }

  void Log::log(int *aAndGBuffer, float *wSpeedBuffer, int *pulseBuffer) {
    beginLog();
    logAccel((int *)&aAndGBuffer);
    logSpeed((float *)&wSpeedBuffer);
    logPulse((int *)&pulseBuffer);
    logFile.println();

    #if debug
      Serial.println();
    #endif
  }
#endif

void Log::logAccel(int *aAndGBuffer) {
  #if full
    int i = 0;
    while(i <= 6) {
      logFile.print(aAndGBugger[i++], DEC);
      logFile.print(",");
      #if debug
        Serial.print(aAndGBuffer[i++]);
        Serial.print(",");
      #endif
    }
  #else
    logFile.print(aAndGBuffer[0]);
    logFile.print(",");
    logFile.print(aAndGBuffer[1]);
    logFile.print(",");
    logFile.print(aAndGBuffer[6]);
    logFile.print(",");

    #if debug
      Serial.print(aAndGBuffer[0]);
      Serial.print(",");
      Serial.print(aAndGBuffer[1]);
      Serial.print(",");
      Serial.print(aAndGBuffer[6]);
      Serial.print(",");
    #endif
  #endif

}

void Log::logSpeed(float *wSpeedBuffer) {
  #if full
    int i = 0;
    while(i <= 3) {
      logFile.print(wSpeedBuffer[i++], DEC);
      logFile.print(",");
      #if debug
        Serial.print(wSpeedBuffer[i++]);
        Serial.print(",");
      #endif
    }
  #else
    logFile.print(wSpeedBuffer[0]);
    logFile.print(",");
    logFile.print(wSpeedBuffer[1]);
    logFile.print(",");
    logFile.print(wSpeedBuffer[2]);
    logFile.print(",");
    logFile.print(wSpeedBuffer[3]);
    logFile.print(",");

    #if debug
      Serial.print(wSpeedBuffer[0]);
      Serial.print(",");
      Serial.print(wSpeedBuffer[1]);
      Serial.print(",");
      Serial.print(wSpeedBuffer[2]);
      Serial.print(",");
      Serial.print(wSpeedBuffer[3]);
      Serial.print(",");
    #endif
  #endif
}

void Log::logPulse(int *pulseBuffer) {
  #if full
    int i = 0;
    while(i <= 2) {
      logFile.print(pulseBuffer[i++], DEC);
      logFile.print(",");
      #if debug
        Serial.print(pulseBuffer[i++]);
        Serial.print(",");
      #endif
    }
  #else
    logFile.print(pulseBuffer[0]);
    logFile.print(",");
    logFile.print(pulseBuffer[1]);
    logFile.print(",");
    logFile.print(pulseBuffer[2]);
    logFile.print(",");

    #if debug
      Serial.print(pulseBuffer[0]);
      Serial.print(",");
      Serial.print(pulseBuffer[1]);
      Serial.print(",");
      Serial.print(pulseBuffer[2]);
      Serial.print(",");
    #endif
  #endif
}
