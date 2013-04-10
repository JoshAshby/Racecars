#ifndef CARD_H
#define CARD_H
#include "Arduino.h"

// Ported to SdFat from the native Arduino SD library example by Bill Greiman
// On the Ethernet Shield, CS is pin 4. SdFat handles setting SS

/*
 SD card read/write
  
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 8
 
 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Bill Greiman 11 Apr 2011
 This example code is in the public domain. 
 */

class Log {
  private:
    SdFat sd;
    SdFile logFile;
    void logAccel(int *aAndGBuffer);
    void logSpeed(float *wSpeedsBuffer);
    void logPulse(int *pulseBuffer);
    
  public:
    Log();
    ~Log();
    void begin();
    void open();
    void close();
    void log(String message);
    void log(String message, int *aAndGBuffer);
    void log(String message, int *aAndGBuffer, float *wSpeedsBuffer);
    void log(String message, int *aAndGBuffer, float *wSpeedsBuffer, int *pulseBuffer);
};

#endif
