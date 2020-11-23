//Colors available to use
#define BLACK           0x00
#define BLUE            0xE0
#define RED             0x03
#define GREEN           0x1C
#define DGREEN          0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define BROWN           0x32


#include <Wire.h>
#include <TinyScreen.h> //include TinyScreen library
#include <TimeLib.h> //include the Arduino Time library
#include <SPI.h>
#include <STBLE.h>
#include <stdio.h>
#include "BMA250.h"


/* For Anti theft alarm */
#ifndef BLE_DEBUG
#define BLE_DEBUG true
#endif
#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#endif

uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
#define PIPE_UART_OVER_BTLE_UART_TX_TX 0

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

TinyScreen display = TinyScreen(TinyScreenDefault); //Create the TinyScreen object
int option = 0;
int eatwhere_setup = 0;
int brightness = 10;

void setup()
{
  Wire.begin();
  display.begin();//Initializes TinyScreen board
  display.setFlip(1);                         //Flips the watch display
  display.on();                               //Turns TinyScreen display on
  display.setBrightness(brightness);          //Set display brightness 0 - 15
  SerialMonitorInterface.begin(9600);
  // Set the time and date. Change this to your current date and time.
  setTime(15,15,55,21,11,2020);    //values in the order hr,min,sec,day,month,year
 
}
 
void loop()
{

  readInput(); //Sets the font size & color of the buttons and also call the buttonLoop function
  switch(option)
  {
    case 0:
    {
      //Sets font size & color for the time by overriding the declarations above
      display.setFont(liberationSansNarrow_14ptFontInfo);   //Set the font type for time
      display.fontColor(WHITE,BLACK);
     
      // display time in HH/MM/SS 24-hours format
      display.setCursor(22,20); //Set the position of the time displayed
      if(hour()<10) display.print(0); //print a leading 0 if hour value is less than 0
      display.print(hour()); //hour() is a built in function to show the hour which was pre-set in setTime()
      display.print(":");
      if(minute()<10) display.print(0); //print a leading 0 if minute value is less than 0
      display.print(minute());
      display.print(":");
      if(second()<10) display.print(0); //print a leading 0 if seconds value is less than 0
      display.print(second());
      display.print(" "); //just an empty space after the seconds
      delay(200); //delay 1 second
      break;
    }
    case 1:
    {
      display.clearScreen();
//      option = countdown();
      display.clearScreen();
      break;
    }
    case 2:
    {
      display.clearScreen();
      option = eatWhere();
      display.clearScreen();
      break;
    }
    case 3:
    {
      display.clearScreen();
      option = anti_theft_alarm();
      display.clearScreen();
      break;
    }
  }
  
}

void readInput() {
  display.fontColor(YELLOW,BLACK); //Set font Color of buttons
  display.setFont(liberationSans_8ptFontInfo); //Set font size of buttons
  buttonLoop();
}

void buttonLoop() {
  display.setCursor(0, 0); //Position of upper left button
  
  if (display.getButtons(TSButtonUpperLeft)) { //Test if a specific button is pressed, getButtons(TSRButtonLowerLeft|TSRButtonLowerRight) to test multiple buttons
    display.println("ok       "); //Spaces are used to cover the original displayed text
    delay(300);
    option = 1;
  } else {
    display.println("Timer");

  }
  display.setCursor(0, 45); //(x-axis,y-axis)
  if (display.getButtons(TSButtonLowerLeft)) {
      delay(300);
      option = 2;
  } else {
    display.println("Random");

  }
  display.setCursor(43, 0);
  if (display.getButtons(TSButtonUpperRight)) {
    if (brightness < 15)
    {
      brightness++;
      display.setBrightness(brightness);
    }
    else if (brightness == 15)
    {
      brightness = 0;
      display.setBrightness(brightness);
    }
  } else {
    display.println("Brightness");
  }
  display.setCursor(65, 45);
  if (display.getButtons(TSButtonLowerRight)) {
//    display.println("      ok");
      delay(300);
      option = 3;
  } else {
    display.println("Alarm");
  }
}
