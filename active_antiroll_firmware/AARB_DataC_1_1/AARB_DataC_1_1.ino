// AARB_DataC_1.1
// Michael Goen 
// Colorado State University Formula SAE
//
// For RR-13
// March 2, 2013
//
#include <Wire.h>
#include <SdFat.h>
#include <Servo.h>
#include "globals.h"
#include "card.h"
#include "ganda.h"

void setup() {
  Logger.begin();
  recording = false;
  
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
  
  //Inputs
  pinMode(selector, INPUT);
  pinMode(steering, INPUT);
  pinMode(speedsensorfl, INPUT);
  pinMode(speedsensorfr, INPUT);
  pinMode(speedsensorrl, INPUT);
  pinMode(speedsensorrr, INPUT);
  pinMode(recswitch, INPUT);
  
  //Output
  pinMode(recled, OUTPUT);
  digitalWrite(recled, LOW);
  
  frontRightServo.attach(sally);
  frontLeftServo.attach(mustang);
  rearServo.attach(bonnieandclyde);
  
  wheelSpeeds.rearRight = 0;
  wheelSpeeds.rearLeft = 0;
  wheelSpeeds.frontRight = 0;
  wheelSpeeds.frontLeft = 0;
}

void loop() {
//  Serial.println(accel_t_gyro.value.x_accel);
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
    pulsewidthfl = pulseIn(speedsensorfl, HIGH, 13900);
    if (pulsewidthfl == 0) {
      wheelSpeeds.frontLeft = 0;
    } else {
      wheelSpeeds.frontLeft = 116025 / pulsewidthfl;
    }
    
    pulsewidthfr = pulseIn(speedsensorfr, HIGH, 13900);
    if (pulsewidthfr == 0) {
      wheelSpeeds.frontRight = 0;
    } else {
      wheelSpeeds.frontRight = 116025 / pulsewidthfr;
    }
  
    pulsewidthrl = pulseIn(speedsensorrl, HIGH, 13900);
    if (pulsewidthrl == 0) {
      wheelSpeeds.rearLeft = 0;
    } else {
      wheelSpeeds.rearLeft = 116025 / pulsewidthrl;
    }
    
    pulsewidthrr = pulseIn(speedsensorrr, HIGH, 13900);
    if (pulsewidthrr == 0) {
      wheelSpeeds.rearRight = 0;
    } else {
      wheelSpeeds.rearRight = 116025 / pulsewidthrr;
    } 
  } else {
    switch(wheel) {
      case 0:
        pulsewidthfl = pulseIn(speedsensorfl, HIGH, 13900);
        if (pulsewidthfl == 0) {
          wheelSpeeds.frontLeft = 0;
        } else {
          wheelSpeeds.frontLeft = 116025 / pulsewidthfl;
        }
        wheel = 1;
        break;
        
      case 1:
        pulsewidthfr = pulseIn(speedsensorfr, HIGH, 13900);
        if (pulsewidthfr == 0) {
          wheelSpeeds.frontRight = 0;
        } else {
          wheelSpeeds.frontRight = 116025 / pulsewidthfr;
        }
        wheel = 2;
        break;
        
      case 2:
        pulsewidthrl = pulseIn(speedsensorrl, HIGH, 13900);
        if (pulsewidthrl == 0) {
          wheelSpeeds.rearLeft = 0;
        } else {
          wheelSpeeds.rearLeft = 116025 / pulsewidthrl;
        }
        wheel = 3;
        break;
        
      case 3:
        pulsewidthrr = pulseIn(speedsensorrr, HIGH, 13900);
        if (pulsewidthrr == 0) {
          wheelSpeeds.rearRight = 0;
        } else {
          wheelSpeeds.rearRight = 116025 / pulsewidthrr;
        }
        wheel = 0;
        break;
    }
  }
  
  //read steering
  steeringangle = analogRead(steering);
  
  //read setting
  setting = analogRead(selector)/33.81;

  switch(setting) {
    case 0:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
    
    case 1:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 2:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
    
    case 3:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 4:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 5:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 6:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
    
    case 7:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 8:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 9:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 10:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 11:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
    
    case 12:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break; 
    
    case 13:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 14:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 15:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 16:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 17:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 18:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;
      
    case 19:
      servos.rear = 50;
      servos.frontRight = 50;
      servos.frontLeft = 50;
      #if full
        percent = "37.0%"
      #endif
      break;

    case 20:
      break;
//      Serial.print("Active");
/*      
      //default setting for low lateral acceleration
      if (accel_t_gyro.value.y_accel < 4095) {
        if (bandcpulse != bandcdefault) {
          bandcpulse = bandcdefault;
          digitalWrite(bonnieandclyde, HIGH);
          delayMicroseconds(bandcpulse);
          digitalWrite(bonnieandclyde, LOW);
        }
        if (sallypulse != sallydefault) {
          sallypulse = sallydefault;
          digitalWrite(sally, HIGH);
          delayMicroseconds(sallypulse);
          digitalWrite(sally, LOW);
        }
        if (mustangpulse != mustangdefault) {
          mustangpulse = mustangdefault;
          digitalWrite(mustang, HIGH);
          delayMicroseconds(mustangpulse);
          digitalWrite(mustang, LOW);
          delay(100);
        }
        return;
      } else {
        //check for and address rear tire lift
        rearslide = wheelspeedrr-wheelspeedrl;
        if (abs(rearslide) > 5) {
          if (bandcpulse < 1925) {
            bandcpulse = bandcpulse + 25;
          }
          if (sallypulse > 1050){
            sallypulse = sallypulse - 25;
          }
          if (mustangpulse > 1050){
            mustangpulse = mustangpulse - 25;
            delay(10);
          }
          return;
        }
        
        if ((wheelspeedfl+wheelspeedfr)/2 < 25) {
          if (accel_t_gyro.value.y_accel > 8190) {
            //At low speeds, shift weight transfer forward during corner exit
            if(accel_t_gyro.value.x_accel > 4095) {
              if (bandcpulse != 975) {
                bandcpulse = 975;
              }
              if (mustangpulse != 1950) {
                mustangpulse = 1950;
              }
              if (sallypulse != 2000) {
                sallypulse = 2000;
                delay(100);
              }
              return;
            }
            
            //At low speeds, shift weight transfer rear during corner entry
            if(accel_t_gyro.value.x_accel < -4095) {
              if (bandcpulse != 975) {
                bandcpulse = 975;
              }
              if (sallypulse != 2000) {
                sallypulse = 2000;
              }
              if (mustangpulse != 1950) {
                mustangpulse = 1950;
                delay(500);
              }
              return;
            }
          }
        }
      } */
  }

  servoWrite(servos);
  #if full
    Logger.log(percent, accel_t_gyro, wheelSpeeds, servos);
  #else
    Logger.log(accel_t_gyro, wheelSpeeds, servos);
  #endif
}
