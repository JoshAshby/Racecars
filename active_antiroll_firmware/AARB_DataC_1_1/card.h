#ifndef CARD_H
#define CARD_H
#include "Arduino.h"
/*
Joshua Ashby 2013
joshuaashby@joshashby.com
Logger object

Overly simplified logger object to make the normal code
cleaner by removing all the log aspects. Also makes the code
easier to work with and modify.

Designed and built with no love for the
2013 CSU FSAE Active Antiroll System by Mike Goen

Uses SdFat
 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Bill Greiman 11 Apr 2011
 */

#define full false
#define debug false

class Log {
  private:
    SdFat sd;
    SdFile logFile;
    void logAccel(int *aAndGBuffer);
    void logSpeed(float *wSpeedsBuffer);
    void logPulse(int *pulseBuffer);
    void beginLog(void);

  public:
    Log();
    ~Log();
    void begin(void);
    void open(void);
    void close(void);
    #if full
      void log(String message);
      void log(String message, int *aAndGBuffer);
      void log(String message, int *aAndGBuffer, float *wSpeedsBuffer);
      void log(String message, int *aAndGBuffer, float *wSpeedsBuffer, int *pulseBuffer);
    #else
      void log(int *aAndGBuffer);
      void log(int *aAndGBuffer, float *wSpeedsBuffer);
      void log(int *aAndGBuffer, float *wSpeedsBuffer, int *pulseBuffer);
    #endif
};

#endif
