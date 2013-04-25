// AARB_DataC_1.1
// Michael Goen 
// Colorado State University Formula SAE
//
// For RR-13
// March 2, 2013
//

//---------------------------------------------------------------------
//                          **Logger settings**
// Put in by Josh Ashby for the card.h and Log object

// First, do you want to enable logging?
// If this is `true` then the system will try to log
#define logging true

// Next, do you want to log only relevant info, or everything?
// If `true` this will log everything
#define full false

// Finally, do you want to log to only the sdcard,
// or also the serial port? If this is `true` then it will
// take the extra time to log to the serial port.
#define debug true

// Next, do you want to have the active setting, which also uses the
// wheel speed sensors and accel/gyro? If this is `true` then setting
// 20 will be the active setting. If `false` then setting 20 is another
// static setting
#define active true
//---------------------------------------------------------------------
// For settings for the wheel encoders, see the bottom of globals.h
//
// There are two variables defined: wheelTimeout and wheelDivisor:
// * wheelTimeout defines the timeout for the pulseIn function
//
// * wheelDivisor defines what number the pulse time is divided by
//     to get speed or rpm
//---------------------------------------------------------------------
//                       **Try not to touch** (things might break)
#include <Wire.h>
#include <SdFat.h>
#include <Servo.h>
#include "globals.h"
#include "card.h"
#include "ganda.h"
//---------------------------------------------------------------------

void setup() {
  #if logging
    Logger.begin();
    recording = false;
    
    pinMode(recswitch, INPUT);
    
    pinMode(recled, OUTPUT);
    digitalWrite(recled, LOW);
  #endif
  
  #if active
    Wire.begin();
    // default at power-up:
    //    Gyro at 250 degrees second
    //    Acceleration at 2g
    //    Clock source at internal 8MHz
    //    The device is in sleep mode.
    error = MPU6050_read(MPU6050_WHO_AM_I, &c, 1);
  
    // According to the datasheet, the 'sleep' bit
    // should read a '1'. But I read a '0'.
    // That bit has to be cleared, since the sensor
    // is in sleep mode at power-up. Even if the
    // bit reads '0'.
    error = MPU6050_read(MPU6050_PWR_MGMT_2, &c, 1);
  
    // Clear the 'sleep' bit to start the sensor.
    MPU6050_write_reg(MPU6050_PWR_MGMT_1, 0);
    MPU6050_write_reg(MPU6050_ACCEL_CONFIG, 0x08);
    MPU6050_write_reg(MPU6050_CONFIG, 0x05);

    pinMode(speedsensorfl, INPUT);
    pinMode(speedsensorfr, INPUT);
    pinMode(speedsensorrl, INPUT);
    pinMode(speedsensorrr, INPUT);

    wheelSpeeds.rearRight = 0;
    wheelSpeeds.rearLeft = 0;
    wheelSpeeds.frontRight = 0;
    wheelSpeeds.frontLeft = 0;
  #endif
  
  //Inputs
  pinMode(selector, INPUT);
  
  frontRightServo.attach(sally);
  frontLeftServo.attach(mustang);
  rearServo.attach(bonnieandclyde);
}

void loop() {
  #if logging
    if(digitalRead(recswitch)==LOW) {
      if(recording == true) {
        recording = false;
        digitalWrite(recled,LOW);
        Logger.close();
      }
    } else {
      if(recording == false) {
        recording = true;
        digitalWrite(recled,HIGH);
        Logger.open();
      }
    }
  #endif
  
  #if active
    // Read the raw values.
    // Read 14 bytes at once, 
    // containing acceleration, temperature and gyro.
    // With the default settings of the MPU-6050,
    // there is no filter enabled, and the values
    // are not very stable.
    error = MPU6050_read(MPU6050_ACCEL_XOUT_H, (uint8_t *)&accel_t_gyro, sizeof(accel_t_gyro));
  
    // Swap all high and low bytes.
    // After this, the registers values are swapped, 
    // so the structure name like x_accel_l does no 
    // longer contain the lower byte.
    SWAP(accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
    SWAP(accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
    SWAP(accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
    SWAP(accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
    SWAP(accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
    SWAP(accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
    SWAP(accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
    
    if (wheelspeedfl >= 20) {
      frontRightWheelSpeed();
      frontLeftWheelSpeed();
      rearRightWheelSpeed();
      rearLeftWheelSpeed();
    } else {
      switch(wheel) {
        case 0:
          frontLeftWheelSpeed();
          wheel = 1;
          break;
          
        case 1:
          frontRightWheelSpeed();
          wheel = 2;
          break;
          
        case 2:
          rearLeftWheelSpeed();
          wheel = 3;
          break;
          
        case 3:
          rearRightWheelSpeed();
          wheel = 0;
          break;
      }
    }
  #endif
   
  //read setting
  reading = analogRead(selector);
  setting = reading/33.81;

  switch(setting) {
    case 0:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full && logging
        percent = "0"
      #endif
      break;
    
    case 1:
      servos.rear = 53;
      servos.frontRight = 53;
      servos.frontLeft = 53;
      #if full && logging
        percent = "1"
      #endif
      break;
      
    case 2:
      servos.rear = 56;
      servos.frontRight = 56;
      servos.frontLeft = 56;
      #if full && logging
        percent = "2"
      #endif
      break;
    
    case 3:
      servos.rear = 60;
      servos.frontRight = 60;
      servos.frontLeft = 60;
      #if full && logging
        percent = "3"
      #endif
      break;
      
    case 4:
      servos.rear = 63;
      servos.frontRight = 63;
      servos.frontLeft = 63;
      #if full && logging
        percent = "4"
      #endif
      break;
      
    case 5:
      servos.rear = 66;
      servos.frontRight = 66;
      servos.frontLeft = 66;
      #if full && logging
        percent = "5"
      #endif
      break;
      
    case 6:
      servos.rear = 70;
      servos.frontRight = 70;
      servos.frontLeft = 70;
      #if full && logging
        percent = "6"
      #endif
      break;
    
    case 7:
      servos.rear = 73;
      servos.frontRight = 73;
      servos.frontLeft = 73;
      #if full && logging
        percent = "7"
      #endif
      break;
      
    case 8:
      servos.rear = 76;
      servos.frontRight = 76;
      servos.frontLeft = 76;
      #if full && logging
        percent = "8"
      #endif
      break;
      
    case 9:
      servos.rear = 80;
      servos.frontRight = 80;
      servos.frontLeft = 80;
      #if full && logging
        percent = "9"
      #endif
      break;
      
    case 10:
      servos.rear = 83;
      servos.frontRight = 83;
      servos.frontLeft = 83;
      #if full && logging
        percent = "10"
      #endif
      break;
      
    case 11:
      servos.rear = 86;
      servos.frontRight = 86;
      servos.frontLeft = 86;
      #if full && logging
        percent = "11"
      #endif
      break;
    
    case 12:
      servos.rear = 90;
      servos.frontRight = 90;
      servos.frontLeft = 90;
      #if full && logging
        percent = "12"
      #endif
      break; 
    
    case 13:
      servos.rear = 93;
      servos.frontRight = 93;
      servos.frontLeft = 93;
      #if full && logging
        percent = "13"
      #endif
      break;
      
    case 14:
      servos.rear = 96;
      servos.frontRight = 96;
      servos.frontLeft = 96;
      #if full && logging
        percent = "14"
      #endif
      break;
      
    case 15:
      servos.rear = 100;
      servos.frontRight = 100;
      servos.frontLeft = 100;
      #if full && logging
        percent = "15"
      #endif
      break;
      
    case 16:
      servos.rear = 103;
      servos.frontRight = 103;
      servos.frontLeft = 103;
      #if full && logging
        percent = "16"
      #endif
      break;
      
    case 17:
      servos.rear = 106;
      servos.frontRight = 106;
      servos.frontLeft = 106;
      #if full && logging
        percent = "17"
      #endif
      break;
      
    case 18:
      servos.rear = 110;
      servos.frontRight = 110;
      servos.frontLeft = 110;
      #if full && logging
        percent = "18"
      #endif
      break;
      
    case 19:
      servos.rear = 113;
      servos.frontRight = 113;
      servos.frontLeft = 113;
      #if full && logging
        percent = "19"
      #endif
      break;

  #if active
    case 20:
      //default setting for low lateral acceleration
      if (accel_t_gyro.value.y_accel > 4095) {
        //check for and address rear tire lift
        rearslide = wheelspeedrr-wheelspeedrl;
        if (abs(rearslide) > 5) {
          servos.frontRight += 2;
          servos.frontLeft += 2;
          servos.rear += 2;
          return;
        }
        
        if ((wheelspeedfl+wheelspeedfr)/2 < 25) {
          if (accel_t_gyro.value.y_accel > 8190) {
            //At low speeds, shift weight transfer forward during corner exit
            if(accel_t_gyro.value.x_accel > 4095) {
              servos.frontRight = 50;
              servos.frontLeft = 50;
              servos.rear = 140;
              return;
            }
            
            //At low speeds, shift weight transfer rear during corner entry
            if(accel_t_gyro.value.x_accel < -4095) {
              servos.frontRight = 50;
              servos.frontLeft = 50;
              servos.rear = 50;
              return;
            }
          }
        }
      }
    #else
      case 20:
        servos.rear = 116;
        servos.frontRight = 116;
        servos.frontLeft = 116;
        #if full && logging
          percent = "20"
        #endif
        break;
    #endif
  }

  servoWrite(servos);
  
  #if logging
    #if full
      Logger.log(percent, accel_t_gyro, wheelSpeeds, servos, reading);
    #else
      Logger.log(accel_t_gyro, wheelSpeeds, servos, reading);
    #endif
  #endif
}
