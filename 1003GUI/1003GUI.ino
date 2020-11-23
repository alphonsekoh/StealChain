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

 
TinyScreen display = TinyScreen(TinyScreenDefault); //Create the TinyScreen object
 
void setup()
{
  Wire.begin();
  display.begin();//Initializes TinyScreen board
  display.setFlip(1);                         //Flips the watch display
  display.on();                               //Turns TinyScreen display on
  display.setBrightness(10);                  //Set display brightness 0 - 15
  
  // Set the time and date. Change this to your current date and time.
  setTime(15,15,55,21,11,2020);    //values in the order hr,min,sec,day,month,year
 
}
 
void loop()
{
  readInput(); //Sets the font size & color of the buttons and also call the buttonLoop function

  
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
  delay(1000); //delay 1 second
  
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
  } else {
    display.println("Timer");

  }
  display.setCursor(0, 45); //(x-axis,y-axis)
  if (display.getButtons(TSButtonLowerLeft)) {
    display.println("ok         ");
  } else {
    display.println("Random");

  }
  display.setCursor(63, 0);
  if (display.getButtons(TSButtonUpperRight)) {
    display.println("       ok");
  } else {
    display.println("Watch");
  }
  display.setCursor(65, 45);
  if (display.getButtons(TSButtonLowerRight)) {
    display.println("      ok");
  } else {
    display.println("Alarm");
  }
}