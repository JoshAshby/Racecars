#include "card.h"
 
//constructor
Log::Log(){
}
 
//destructor
Log::~Log(){
}

void Log::begin() {
  //Start up serial, so we can constantly log to it.
  Serial.begin(9600);
}

void Log::open() {
  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
    Serial.println("Failed to connect to card, skipping logging to microSD...");
  }

  // open the file for write at end like the Native SD library
  if (!logFile.open("log.csv", O_RDWR | O_CREAT | O_AT_END)) {
    Serial.println("Failed to open log.csv, skipping microSD logging...");
  } else {
    // if the file opened okay, write to it:
    Serial.println("Logging to log.csv...");
    logFile.println("time,xaccel,yaccel,zaccel,temp,xgyro,ygyro,zgyro,frspeed,flspeed,rrspeed,rlspeed,sally,mustang,bandc,message");
  }
}

void Log::close() {
  // close the file:
  logFile.close();
}

void Log::log(String message) {
  logFile.println("**Begin Error:: " + message + " ::End Error**");
}

void Log::log(String message, int *aAndGBuffer) {
  unsigned long time = millis();
  logFile.print(time, DEC);
  Serial.print(time);
  logFile.print(",");
  Serial.print(",");
  logAccel((int *)&aAndGBuffer);
  logFile.println(message);
  Serial.println(message);
}

void Log::log(String message, int *aAndGBuffer, float *wSpeedBuffer) {
  unsigned long time = millis();
  logFile.print(time, DEC);
  Serial.print(time);
  logFile.print(",");
  Serial.print(",");
  logAccel((int *)&aAndGBuffer);
  logSpeed((float *)&wSpeedBuffer);
  logFile.println(message);
  Serial.println(message);
}

void Log::log(String message, int *aAndGBuffer, float *wSpeedBuffer, int *pulseBuffer) {
  unsigned long time = millis();
  logFile.print(time, DEC);
  Serial.print(time);
  logFile.print(",");
  Serial.print(",");
  logAccel((int *)&aAndGBuffer);
  logSpeed((float *)&wSpeedBuffer);
  logPulse((int *)&pulseBuffer);
  logFile.println(message);
  Serial.println(message);
}

void Log::logAccel(int *aAndGBuffer) {
  int i = 0;
  while(i <= 6) {
    Serial.print(aAndGBuffer[i++], DEC);
    Serial.print(",");
  }
}

void Log::logSpeed(float *wSpeedBuffer) {
  int i = 0;
  while(i <= 3) {
    Serial.print(wSpeedBuffer[i++], DEC);
    Serial.print(","); 
  }
}

void Log::logPulse(int *pulseBuffer) {
  int i = 0;
  while(i <= 2) {
    Serial.print(pulseBuffer[i++], DEC);
    Serial.print(",");
  }
}
