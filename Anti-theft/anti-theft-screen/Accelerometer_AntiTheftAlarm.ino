#include <Wire.h>        // For I2C communication with sensor
#include "BMA250.h"      // For interfacing with the accel. sensor
#include <TinyScreen.h>  // tinyscreen library
#define SerialMonitorInterface SerialUSB



int delaytime = 1000;
BMA250 accel_sensor;
int x, y, z;
int x2, y2, z2;
int x3, y3, z3;


// Accelerometer sensor variables for the sensor and its values
  

unsigned long pretime = 0;
int shown = 0;
void blinkAlert(){
   if(millis() >= pretime+1000)
  {
    pretime = millis();
    SerialMonitorInterface.println("Clear screen");
    
    if(shown)
    {
      display.clearScreen();
      shown = 0;
    }
    else
    {
    shown = 1;
       display.drawRect(0,0,95,63,1,TS_8b_Red);
      display.setCursor(25,20);       //set the cursor to x, y position, where to start next print/draw instruction
     display.setFont(liberationSansNarrow_12ptFontInfo);
     display.fontColor(TS_8b_Black,TS_8b_Red);
      display.print("ALERT!!");
    }
  }
}

void readOriginal(){
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
  accel_sensor.read();//This function gets new data from the acccelerometer
  // Get the acceleration values from the sensor and store them into global variables
  // (Makes reading the rest of the program easier)
  x = accel_sensor.X;
  y = accel_sensor.Y;
  z = accel_sensor.Z;
  
  
  }

void readUpdated(){
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
  accel_sensor.read();//This function gets new data from the acccelerometer
  // Get the acceleration values from the sensor and store them into global variables
  // (Makes reading the rest of the program easier)
  x2 = accel_sensor.X;
  y2 = accel_sensor.Y;
  z2 = accel_sensor.Z;
  }

void diffInReading(){
    x3 = x - x2;
    y3 = y - y2;
    z3 = z - z2;
}

int startblink = 0;
int willItBlink(){
  if (startblink)
  {
    blinkAlert();
    
  }else
  {
    if(x3>50 || x3<-50 || y3>50 || y3<-50){
        startblink = 1;
        return 1;
     }
  }
  return 0;
  //delay(250);
}
void stopBlink()
{
  startblink = 0;
}


  
