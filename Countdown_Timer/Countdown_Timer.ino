#include <RTCZero.h>
#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <string.h>
#include <stdbool.h>

RTCZero RTCZ;

int choose_place = 0;
long randIndex;

TinyScreen display = TinyScreen(TinyScreenDefault);

const uint8_t displayStateHome = 0x01;
const uint8_t displayStateMenu = 0x02;
const uint8_t displayStateEditor = 0x03;
uint8_t defaultFontColor = TS_8b_White;
uint8_t defaultFontBG = TS_8b_Black;
uint8_t inactiveFontColor = TS_8b_Gray;
uint8_t inactiveFontBG = TS_8b_Black;
const FONT_INFO &clockFont = liberationSansNarrow_22ptFontInfo;
uint8_t timeY = 20;
unsigned long millisOffsetCount = 0;

int hours = 0;
int minutes = 0;
int seconds = 0;
int totalSeconds = 0;

bool rewriteTime = true;
bool isPaused = false;
bool isInSettings = false;

uint8_t currentDisplayState = displayStateHome;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#include <TimeLib.h>

#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#include <RTCZero.h>
#include <time.h>
#endif

void setup()
{
  Wire.begin(); //initialize I2C before we can initialize TinyScreen
  SerialMonitorInterface.begin(9600);
  display.begin();
  display.on();
  display.setFlip(true);
  display.setBrightness(10); //sets main current level, valid levels are 0-15
  display.setFont(liberationSans_8ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();
  display.setCursor(5, 25);
  //Display the content
  displayMenu();
  hours = 1;
  minutes = 00;
  seconds = 40;
}

void updateMainDisplay()
{
  if (currentDisplayState == displayStateHome)
  {
    updateTimeDisplay();
  }
}

void updateTimeDisplay()
  {
//    int currentHour, currentMinute, currentSecond;
//    currentHour = 0;
//    currentMinute = 0;
//    currentSecond = 0;

    totalSeconds = hours * 3600 + minutes * 60 + seconds;
    
    char displayX;

    display.clearWindow(0, 48, 64, 11);
    display.setCursor(0, 48);
    display.print("< Pause");

    isPaused = false;

    while (totalSeconds > 0)
    {
      
      int internalSeconds = totalSeconds;
      int secondsToDisplay = 0;
      int minutesToDisplay = 0;
      int hoursToDisplay = 0;
      
      if (internalSeconds >= 3600)
      {
        hoursToDisplay = internalSeconds/3600;
        internalSeconds -= hoursToDisplay * 3600;
      }
      if (internalSeconds >= 60)
      {
        minutesToDisplay = internalSeconds/60;
        internalSeconds -= minutesToDisplay * 60;
      }
      
      secondsToDisplay = internalSeconds;

      if (display.getButtons(TSButtonUpperLeft))
      {
        totalSeconds = 0;
      }
      else if (display.getButtons(TSButtonLowerLeft))
      {
        hours = hoursToDisplay;
        minutes = minutesToDisplay;
        seconds = secondsToDisplay;
        isPaused = true;
        display.clearWindow(0, 48, 64, 11);
        display.setFont(liberationSans_8ptFontInfo);
        display.fontColor(TS_8b_White, TS_8b_Black);
        display.setCursor(0, 48);
        delay(50);
        display.print("< Continue");
        break;
      }
      else if (display.getButtons(TSButtonUpperRight))
      {
        //Set button has been called
        hours = hoursToDisplay;
        minutes = minutesToDisplay;
        seconds = secondsToDisplay;
        isInSettings = true;
        adjustTimer();
        break;
      }

    display.setFont(clockFont);
    displayX = 0;
    display.setCursor(displayX, timeY);
    if (hoursToDisplay < 10)
    display.print('0');
    display.print(hoursToDisplay);
    display.write(':');

    display.setFont(clockFont);
    displayX = 14 + 14 - 1;
    display.setCursor(displayX, timeY);
    if (minutesToDisplay < 10)
      display.print('0');
    display.print(minutesToDisplay);
    display.write(':');

    display.setFont(clockFont);
    displayX = 14 + 14 + 14 + 14 - 2;
    display.setCursor(displayX, timeY);
    if (secondsToDisplay < 10)
      display.print('0');
    display.print(secondsToDisplay);

      delay(100);
      totalSeconds--;
    }

    
}

void adjustTimer()
{
  char displayX;
  
  display.clearWindow(0, 0, 96, 11);
  display.setFont(liberationSans_8ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.setCursor(0, 0);
  delay(200);
  display.print("< Next");

  display.setCursor(90, 0);
  delay(200);
  display.print("^");

  display.setCursor(90, 48);
  delay(200);
  display.print("v");

  bool hoursSet = false;
  bool minsSet = false;
  bool secondsSet = false;

while (!display.getButtons(TSButtonUpperLeft))
{
  //For the seconds
  while (!secondsSet)
  {
    //For the Mins
    while (!minsSet)
    {
      //For the Hours
      while (!hoursSet)
      {
        delay(200);
        if (display.getButtons(TSButtonUpperLeft))
        {
          hoursSet = true;
          break;
        }

        switch (display.getButtons()) 
        {
          //Increment the value
          case TSButtonUpperRight:
          hours++;
          delay(200);
          break;
    
          //Decrement the value
          case TSButtonLowerRight:
          hours--;
          delay(200);
          break;
        }
    
        display.setFont(clockFont);
        display.fontColor(defaultFontColor, defaultFontBG);
        displayX = 0;
        display.setCursor(displayX, timeY);
        if (hours < 10)
        display.print('0');
        display.print(hours);
        display.write(':');
      
        display.setFont(clockFont);
        display.fontColor(inactiveFontColor, inactiveFontBG);
        displayX = 14 + 14 - 1;
        display.setCursor(displayX, timeY);
        if (minutes < 10)
          display.print('0');
        display.print(minutes);
        display.write(':');
      
        display.setFont(clockFont);
        display.fontColor(inactiveFontColor, inactiveFontBG);
        displayX = 14 + 14 + 14 + 14 - 2;
        display.setCursor(displayX, timeY);
        if (seconds < 10)
        display.print('0');
        display.print(seconds);
     }

      //handling the minutes here
      delay(200);
      if (display.getButtons(TSButtonUpperLeft))
      {
        minsSet = true;
        break;
      }

      switch (display.getButtons()) 
      {
        //Increment the value
        case TSButtonUpperRight:
        minutes++;
        if (minutes > 59)
        {
          hours++;
          minutes -= 60;
        }
        delay(200);
        break;
  
        //Decrement the value
        case TSButtonLowerRight:
        if (minutes > 0)
        {
          minutes--;        
        }
        delay(200);
        break;
      }
  
      display.setFont(clockFont);
      display.fontColor(inactiveFontColor, inactiveFontBG);
      displayX = 0;
      display.setCursor(displayX, timeY);
      if (hours < 10)
      display.print('0');
      display.print(hours);
      display.write(':');
    
      display.setFont(clockFont);
      display.fontColor(defaultFontColor, defaultFontBG);
      displayX = 14 + 14 - 1;
      display.setCursor(displayX, timeY);
      if (minutes < 10)
        display.print('0');
      display.print(minutes);
      display.write(':');
    
      display.setFont(clockFont);
      display.fontColor(inactiveFontColor, inactiveFontBG);
      displayX = 14 + 14 + 14 + 14 - 2;
      display.setCursor(displayX, timeY);
      if (seconds < 10)
      display.print('0');
      display.print(seconds);
    }

    //handling the seconds starts here
    delay(200);
    display.clearWindow(0, 0, 96, 11);
    display.setFont(liberationSans_8ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.setCursor(0, 0);
    display.print("< Save");
    
    if (display.getButtons(TSButtonUpperLeft))
    {
      secondsSet = true;
      break;
    }

    switch (display.getButtons()) 
    {
      //Increment the value
      case TSButtonUpperRight:
      seconds++;
      if (seconds > 59)
      {
        minutes++;
        seconds -= 60;
      }
      delay(200);
      break;

      //Decrement the value
      case TSButtonLowerRight:
      if (seconds > 0)
      {
        seconds--;      
      }
      delay(200);
      break;
    }

    display.setFont(clockFont);
    display.fontColor(inactiveFontColor, inactiveFontBG);
    displayX = 0;
    display.setCursor(displayX, timeY);
    if (hours < 10)
    display.print('0');
    display.print(hours);
    display.write(':');
  
    display.setFont(clockFont);
    display.fontColor(inactiveFontColor, inactiveFontBG);
    displayX = 14 + 14 - 1;
    display.setCursor(displayX, timeY);
    if (minutes < 10)
      display.print('0');
    display.print(minutes);
    display.write(':');
  
    display.setFont(clockFont);
    display.fontColor(defaultFontColor, defaultFontBG);
    displayX = 14 + 14 + 14 + 14 - 2;
    display.setCursor(displayX, timeY);
    if (seconds < 10)
    display.print('0');
    display.print(seconds);
    
  }
  //Save button triggered
  if (display.getButtons(TSButtonUpperLeft))
  {
    delay(200);
    displayMenu();
    updateTimeDisplay();
    break;
  }
  delay(200);
}

//  display.setFont(clockFont);
//  displayX = 0;
//  display.setCursor(displayX, timeY);
//  if (hoursToDisplay < 10)
//  display.print('0');
//  display.print(hoursToDisplay);
//  display.write(':');
//
//  display.setFont(clockFont);
//  displayX = 14 + 14 - 1;
//  display.setCursor(displayX, timeY);
//  if (minutesToDisplay < 10)
//    display.print('0');
//  display.print(minutesToDisplay);
//  display.write(':');
//
//  display.setFont(clockFont);
//  displayX = 14 + 14 + 14 + 14 - 2;
//  display.setCursor(displayX, timeY);
//  if (secondsToDisplay < 10)
//    display.print('0');
//  display.print(secondsToDisplay);
}


int countdown(int hours, int minutes, int seconds)
{
  uint8_t lastHourDisplayed = -1;
  uint8_t lastMinuteDisplayed = -1;
  uint8_t lastSecondDisplayed = -1;
  
//    while (hours > 0 || minutes > 0 || seconds > 0)
//  {
//    int internalSeconds = seconds;
//    int hourToPrint = 0;
//    int minToPrint = 0;
//    int secToPrint = 0;
//    if (seconds/3600 > 1)
//    {
//      hourToPrint = seconds/3600;
//      
//    }
//    
//    lastHourDisplayed = seconds/3600;
//    lastMinuteDisplayed = seconds/60;
//
//      
//    display.setFont(clockFont);
//    lastHourDisplayed = 0;
//    displayX = 0;
//    display.setCursor(displayX, timeY);
//    if (lastHourDisplayed < 10)
//      display.print('0');
//    display.print(lastHourDisplayed);
//    display.write(':');
//
//    display.setFont(clockFont);
//    lastMinuteDisplayed = minute;
//    displayX = 14 + 14 - 1;
//    display.setCursor(displayX, timeY);
//    if (lastMinuteDisplayed < 10)
//      display.print('0');
//    display.print(lastMinuteDisplayed);
//    display.write(':');
//
//    display.setFont(clockFont);
//    lastSecondDisplayed = seconds;
//    displayX = 14 + 14 + 14 + 14 - 2;
//    display.setCursor(displayX, timeY);
//    if (lastSecondDisplayed < 10)
//      display.print('0');
//    display.print(lastSecondDisplayed);
//
//    seconds--;
//
//    delay(1000);
//  }
}

    void loop()
    {
      if(!isPaused || !isInSettings)
      {
        updateTimeDisplay();
      }
      //listenForButtonPress();
      if (display.getButtons(TSButtonLowerLeft) && isPaused)
      {
        //Continue button is triggered
        display.clearWindow(0, 48, 64, 11);
        display.setCursor(0, 48);
        display.print("< Pause");
        updateTimeDisplay();
      }
      else if (display.getButtons(TSButtonUpperRight))
      {
        //Set button is triggered
        
      }

 }



    void listenForButtonPress()
    {
      if (display.getButtons(TSButtonLowerLeft))
      {
        //Toggle start/stop
      }
      else if (display.getButtons(TSButtonUpperLeft))
      {
        //Reset timer..
        hours = 0;
        minutes = 0;
        seconds = 0;
        totalSeconds = 0;
      }

      
    }
//
//    void choosePlace()
//    {
//      if (display.getButtons(TSButtonUpperLeft))
//      {
//        for (int i = 0; i < 10; i++)
//        {
//          delay(200);
//          display.clearScreen();
//          display.setCursor(5, 25);
//          display.print(canteens[random(5)]);
//        }
//        display.clearScreen();
//        display.setCursor(5, 25);
//        randIndex = random(5);
//        display.print(canteens[(int)randIndex]);
//        choose_place = 1;
//      }
//    }

//    void chooseStall()
//    {
//      if (display.getButtons(TSButtonUpperLeft))
//      {
//
//        display.clearScreen();
//        display.setCursor(5, 25);
//        if (randIndex == 0)
//          display.print(north_canteen[random(5)]);
//        else if (randIndex == 1)
//          display.print(south_canteen[random(8)]);
//        else if (randIndex == 2)
//          display.print(koufu[random(11)]);
//        else if (randIndex == 3)
//          display.print(foogle[random(7)]);
//        else if (randIndex == 4)
//          display.print(food_connect[random(3)]);
//      }
//    }

    void displayMenu()
    {
      display.setCursor(0, 0);
      display.print("< Reset");
      display.setCursor(0, 48);
      display.print("< Start/Stop");
      display.setCursor(66, 0);
      display.print("Set >");
    }
//
//    void displayMenu2()
//    {
//      display.setCursor(0, 0);
//      display.print("Choose Stall");
//    }
