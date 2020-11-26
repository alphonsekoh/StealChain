#include <Wire.h>        // For I2C communication with sensor
#include "BMA250.h"      // For interfacing with the accel. sensor
#include <TinyScreen.h>  // tinyscreen library
#define SerialMonitorInterface SerialUSB


// Accelerometer sensor variables for the sensor and its values
int delaytime = 1000;
BMA250 accel_sensor;
int x, y, z;
int x2, y2, z2;
int x3, y3, z3;

  

unsigned long pretime = 0;
int shown = 0;
// This function will blink/flash a red display repeatedly
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
       display.drawRect(0,0,95,63,1,TS_8b_Red);          // set the entire display to red color
      display.setCursor(25,20);                          //set the cursor to x, y position, where to start next print/draw instruction
     display.setFont(liberationSansNarrow_12ptFontInfo); //set font type and size
     display.fontColor(TS_8b_Black,TS_8b_Red);           //set font color
      display.print("ALERT!!");
    }
  }
}

//This function gets initial reading data from the acccelerometer
// Get the acceleration values from the sensor and store them into global variables
// (Makes reading the rest of the program easier)
void readOriginal(){
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); // get the sensor to start
  accel_sensor.read();                                          // get the sensor reading
  x = accel_sensor.X;                                           // set the value of X dimension using sensor
  y = accel_sensor.Y;                                           // set the value of y dimension using sensor
  z = accel_sensor.Z;                                           // set the value of z dimension using sensor
  
  
  }

//This function gets new reading data from the acccelerometer
// Get the acceleration values from the sensor and store them into global variables
// (Makes reading the rest of the program easier)
void readUpdated(){
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); // get the sensor to start
  accel_sensor.read();                                          // get the sensor reading
  x2 = accel_sensor.X;                                          // set the value of X dimension using sensor
  y2 = accel_sensor.Y;                                          // set the value of Y dimension using sensor
  z2 = accel_sensor.Z;                                          // set the value of X dimension using sensor
  }

//This function gets the difference in value of the initial reading and the updated reading
void diffInReading(){
    x3 = x - x2;  // get the difference in initial x value and updated x value and set it into new variable
    y3 = y - y2;  // get the difference in initial y value and updated y value and set it into new variable
    z3 = z - z2;  // get the difference in initial z value and updated z value and set it into new variable
}


int startblink = 0;
// This function is used to determine the range of the reading. If it went over a certain range, it will blink.
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


  
