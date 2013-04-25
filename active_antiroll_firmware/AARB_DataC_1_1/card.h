#ifndef CARD_H
#define CARD_H
#include "Arduino.h"
#include "typedefs.h"
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
    void error(String message);
    void log(String message);
    #if active
      #if full
        void log(String message, const accel_t_gyro_union& aAndG, const speeds_struct& speeds, const servos_struct&  servos, float reading);
      #else
        void log(const accel_t_gyro_union& aAndG, const speeds_struct& speeds, const servos_struct&  servos, float reading);
      #endif
    #endif
      
};

#endif
