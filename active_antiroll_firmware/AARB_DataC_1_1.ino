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
#include "card.h"
#include "ganda.h"

// Declaring an union for the registers and the axis values.
// The byte order does not match the byte order of 
// the compiler and AVR chip.
// The AVR chip (on the Arduino board) has the Low Byte 
// at the lower address.
// But the MPU-6050 has a different order: High Byte at
// lower address, so that has to be corrected.
// The register part "reg" is only used internally, 
// and are swapped in code.
typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int x_accel;
    int y_accel;
    int z_accel;
    int temperature;
    int x_gyro;
    int y_gyro;
    int z_gyro;
  } value;
};

typedef union wheelSpeeds_struct
{
  struct
  {
    float frontRight;
    float frontLeft;
    float rearRight;
    float rearLeft;
  } speeds;
};

typedef union pulse_struct
{
  struct
  {
    int frontRight;
    int frontLeft;
    int rear;
  } pulses;
};

int wheel = 0;
int selector = A0;
int steering = A1;
int speedsensorfl = A2;
int speedsensorfr = A3;
int speedsensorrl = 5;
int speedsensorrr = 6;
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
boolean recording;
float steeringangle;
unsigned long time;

int error;
uint8_t c;

double dT;
accel_t_gyro_union accel_t_gyro;
wheelSpeeds_struct wheelSpeeds;
pulse_struct servos;

const int chipSelect = 8;

uint8_t swap;
#define SWAP(x,y) swap = x; x = y; y = swap
  
Log Logger;

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
}

void loop() {
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
      wheelSpeeds.speeds.frontLeft = 0;
    } else {
      wheelSpeeds.speeds.frontLeft = 116025 / pulsewidthfl;
    }
    
    pulsewidthfr = pulseIn(speedsensorfr, HIGH, 13900);
    if (pulsewidthfr == 0) {
      wheelSpeeds.speeds.frontRight = 0;
    } else {
      wheelSpeeds.speeds.frontRight = 116025 / pulsewidthfr;
    }
  
    pulsewidthrl = pulseIn(speedsensorrl, HIGH, 13900);
    if (pulsewidthrl == 0) {
      wheelSpeeds.speeds.rearLeft = 0;
    } else {
      wheelSpeeds.speeds.rearLeft = 116025 / pulsewidthrl;
    }
    
    pulsewidthrr = pulseIn(speedsensorrr, HIGH, 13900);
    if (pulsewidthrr == 0) {
      wheelSpeeds.speeds.rearRight = 0;
    } else {
      wheelSpeeds.speeds.rearRight = 116025 / pulsewidthrr;
    } 
  } else {
    switch(wheel) {
      case 0:
        pulsewidthfl = pulseIn(speedsensorfl, HIGH, 13900);
        if (pulsewidthfl == 0) {
          wheelSpeeds.speeds.frontLeft = 0;
        } else {
          wheelSpeeds.speeds.frontLeft = 116025 / pulsewidthfl;
        }
        wheel = 1;
        break;
        
      case 1:
        pulsewidthfr = pulseIn(speedsensorfr, HIGH, 13900);
        if (pulsewidthfr == 0) {
          wheelSpeeds.speeds.frontRight = 0;
        } else {
          wheelSpeeds.speeds.frontRight = 116025 / pulsewidthfr;
        }
        wheel = 2;
        break;
        
      case 2:
        pulsewidthrl = pulseIn(speedsensorrl, HIGH, 13900);
        if (pulsewidthrl == 0) {
          wheelSpeeds.speeds.rearLeft = 0;
        } else {
          wheelSpeeds.speeds.rearLeft = 116025 / pulsewidthrl;
        }
        wheel = 3;
        break;
        
      case 3:
        pulsewidthrr = pulseIn(speedsensorrr, HIGH, 13900);
        if (pulsewidthrr == 0) {
          wheelSpeeds.speeds.rearRight = 0;
        } else {
          wheelSpeeds.speeds.rearRight = 116025 / pulsewidthrr;
        }
        wheel = 0;
        break;
    }
  }
  
  //read steering
  steeringangle = analogRead(steering);
  
  //read setting
  setting = analogRead(selector)/33.81;

/*
  switch(setting) {
    case 0:
      Serial.print("37.0%");
        bandcpulse = 975;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
    
    case 1:
      Serial.print("37.0%");
        bandcpulse = 975;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
      
    case 2:
      Serial.print("39.4%");
        bandcpulse = 986;
        sallypulse = 1409;
        mustangpulse = 1359;
      break;
    
    case 3:
      Serial.print("40.2%");
        bandcpulse = 1028;
        sallypulse = 1354;
        mustangpulse = 1324;
      break;
      
    case 4:
      Serial.print("41.0%");
        bandcpulse = 1291;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
      
    case 5:
      Serial.print("41.8%");
        bandcpulse = 1362;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
      
    case 6:
      Serial.print("42.6%");
        bandcpulse = 1598;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
    
    case 7:
      Serial.print("43.4%");
        bandcpulse = 1900;
        sallypulse = 2000;
        mustangpulse = 1950;
      break;
      
    case 8:
      Serial.print("44.2%");
        bandcpulse = 1900;
        sallypulse = 1637;
        mustangpulse = 1587;
      break;
      
    case 9:
      Serial.print("45.0%");
        bandcpulse = 1900;
        sallypulse = 1520;
        mustangpulse = 1470;
      break;
      
    case 10:
      Serial.print("45.8%");
        bandcpulse = 1900;
        sallypulse = 1446;
        mustangpulse = 1396;
      break;
      
    case 11:
      Serial.print("46.6%");
        bandcpulse = 1900;
        sallypulse = 1358;
        mustangpulse = 1338;
      break;
    
    case 12:
      Serial.print("47.4%");
        bandcpulse = 1900;
        sallypulse = 1342;
        mustangpulse = 1292;
      break; 
    
    case 13:
      Serial.print("48.2%");
        bandcpulse = 1900;
        sallypulse = 1303;
        mustangpulse = 1253;
      break;
      
    case 14:
      Serial.print("49.0%");
        bandcpulse = 1238;
        sallypulse = 1134;
        mustangpulse = 1084;
      break;
      
    case 15:
      Serial.print("49.8%");
        bandcpulse = 1270;
        sallypulse = 1113;
        mustangpulse = 1063;
      break;
      
    case 16:
      Serial.print("50.6%");
        bandcpulse = 1327;
        sallypulse = 1103;
        mustangpulse = 1053;
      break;
      
    case 17:
      Serial.print("51.4%");
        bandcpulse = 1520;
        sallypulse = 1103;
        mustangpulse = 1053;
      break;
      
    case 18:
      Serial.print("52.2%");
        bandcpulse = 1592;
        sallypulse = 1103;
        mustangpulse = 1053;
      break;
      
    case 19:
      Serial.print("53.0%");
        bandcpulse = 1900;
        sallypulse = 1050;
        mustangpulse = 1000;
      break;
      
    case 20:
      Serial.print("Active");
      
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
      }
  }
  */
  servoWrite((int *)&servos);
  Logger.log("Normal", (int *)&accel_t_gyro, (float *)&wheelSpeeds, (int *)&servos);
}
