#include <Wire.h>        // For I2C communication with sensor
#include "BMA250.h"      // For interfacing with the accel. sensor
#include <TinyScreen.h>  // tinyscreen library

TinyScreen display = TinyScreen(TinyScreenDefault);

int delaytime = 1000;

// Accelerometer sensor variables for the sensor and its values
BMA250 accel_sensor;
int x, y, z;
int x2, y2, z2;
int x3, y3, z3;

void setup() {
  Wire.begin();
  display.begin();
  display.setFlip(1);
  display.on();
  display.setBrightness(10);
  
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms);
  readOriginal();
  
}

void loop() {
  readUpdated();
  diffInReading();
  willItBlink();
}


void blinkAlert(){
  display.drawRect(0,0,95,63,1,TS_8b_Red);
  display.setCursor(15,8);       //set the cursor to x, y position, where to start next print/draw instruction
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.fontColor(TS_8b_Black,TS_8b_Red);
  display.print("Help!!");  //print command
  delay(delaytime);                    //delay for 1000 milliseconds or one second
  display.clearScreen();          //clears the screen blank
  delay(delaytime); 
}

void readOriginal(){
  accel_sensor.read();//This function gets new data from the acccelerometer
  // Get the acceleration values from the sensor and store them into global variables
  // (Makes reading the rest of the program easier)
  x = accel_sensor.X;
  y = accel_sensor.Y;
  z = accel_sensor.Z;
  }

void readUpdated(){
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

void willItBlink(){
  if(x3>20 || x3<-20 || y3>20 || y3<-20){
      blinkAlert();
      }
  delay(250);
  }


  
