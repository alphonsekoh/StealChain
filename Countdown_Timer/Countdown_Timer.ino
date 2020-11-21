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

char *canteens[5] = {"North Canteen", "South Canteen", "Koufu", "Foogle Hub", "Food Connect"};
char *north_canteen[5] = {"Western Cuisine", "Pasta", "Economical Rice", "Thai Cuisine", "Indo Delight"};
char *south_canteen[8] = {"Muslim Food", "Vegetarian", "Chicken Rice", "Indian Food", "Japanese Food", "Ban Mian", "Economy Rice", "Western Food"};
char *koufu[11] = {"Fishball Noodle", "Yong Tau Foo", "Thai Cuisine", "Korean Cuisine", "Nasi Padang", "Curry Rice", "Western", "Japanese Cuisine", "Vegetarian", "Turkish Kebab", "Mala Hotpot"};
char *foogle[7] = {"Fusion Food", "Ayam Penyet", "Japanese/Korean", "Thai Cuisine", "Chicken Rice", "Mala Hotpot", "Indian Muslim"};
char *food_connect[3] = {"Manna", "Subway", "International Mart"};

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
  updateTimeDisplay();
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
    int currentHour, currentMinute, currentSecond;
    currentHour = 0;
    currentMinute = 0;
    currentSecond = 0;

    hours = 1;
    minutes = 00;
    seconds = 40;

    totalSeconds = hours * 3600 + minutes * 60 + seconds;
    
    char displayX;

    display.clearWindow(0, 48, 64, 11);
    display.setCursor(0, 48);
    display.print("< Pause");

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

      delay(1000);
      totalSeconds--;
    }

    
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
      if (choose_place == 0)
      {
        choosePlace();
      }
      else
      {
        displayMenu2();
        chooseStall();
      }
    }

    void choosePlace()
    {
      if (display.getButtons(TSButtonUpperLeft))
      {
        for (int i = 0; i < 10; i++)
        {
          delay(200);
          display.clearScreen();
          display.setCursor(5, 25);
          display.print(canteens[random(5)]);
        }
        display.clearScreen();
        display.setCursor(5, 25);
        randIndex = random(5);
        display.print(canteens[(int)randIndex]);
        choose_place = 1;
      }
    }

    void chooseStall()
    {
      if (display.getButtons(TSButtonUpperLeft))
      {

        display.clearScreen();
        display.setCursor(5, 25);
        if (randIndex == 0)
          display.print(north_canteen[random(5)]);
        else if (randIndex == 1)
          display.print(south_canteen[random(8)]);
        else if (randIndex == 2)
          display.print(koufu[random(11)]);
        else if (randIndex == 3)
          display.print(foogle[random(7)]);
        else if (randIndex == 4)
          display.print(food_connect[random(3)]);

        //    for(int i=0; i<10; i++)
        //    {
        //      delay(200);
        //      display.clearScreen();
        //      display.setCursor(5,25);
        //      display.print(random(0,4));
        //    }
      }
    }

    void displayMenu()
    {
      display.setCursor(0, 0);
      display.print("< Reset");
      display.setCursor(0, 48);
      display.print("< Start/Stop");
      display.setCursor(66, 0);
      display.print("Set >");
    }

    void displayMenu2()
    {
      display.setCursor(0, 0);
      display.print("Choose Stall");
    }
