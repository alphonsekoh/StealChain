#include <Wire.h>
#include <TinyScreen.h>
#include "BMA250.h"
#define SerialMonitorInterface SerialUSB


TinyScreen display = TinyScreen(TinyScreenDefault);
char* anti_theft = "OFF";
int blink = 0;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  display.begin();                            //Initializes TinyScreen board
  display.setFlip(1);                         //Flips the TinyScreen rightside up for O Watch
  display.on();                               //Turns TinyScreen display on
  display.setBrightness(10);

  default_display();
  
}

void loop() {
  
  if(blink)
  {
    SerialMonitorInterface.println("Blink");
    blinkAlert();
  }
  
  // put your main code here, to run repeatedly:
  switch (display.getButtons())
  {
    case TSButtonUpperLeft:
      delay(300);
      display.clearScreen();
      if (strcmp(anti_theft, "OFF") == 0)
      {
        anti_theft = "ON";
        ON(anti_theft);
        blink = 1;
      }else
      {
        anti_theft = "OFF";
        OFF(anti_theft);
        blink = 0;
      }
      break;
      
    case TSButtonLowerLeft:
      if (blink)
      {
        delay(300);
        display.clearScreen();
        blink = 0;
        anti_theft = "OFF";
        default_display();
      }else
      {
        back();
      }
      break;
      
    case TSButtonLowerRight:
       delay(300);
       display.clearScreen();
       blink = 0;
       anti_theft = "OFF";
       default_display();
       break;
       
    case TSButtonUpperRight:
       delay(300);
       display.clearScreen();
       blink = 0;
       anti_theft = "OFF";
       default_display();
       break;
  }


}

void default_display()
{
  //Default 
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0,0);
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print("< ON");

  display.fontColor(TS_8b_White,TS_8b_Black);
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.setCursor(20,20);
  display.print("Anti Theft");
  
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(35,35);
  display.fontColor(TS_8b_Red,TS_8b_Black);
  display.print(anti_theft);

  back();
}

void ON(char* anti_theft){
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(0,0);
    display.fontColor(TS_8b_White,TS_8b_Black);
    display.print("< OFF");
    display.fontColor(TS_8b_White,TS_8b_Black);
    display.setFont(liberationSansNarrow_12ptFontInfo);
    display.setCursor(20,20);
    display.print("Anti Theft");
    display.clearWindow(35,35,96,44);
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(35,35);
    display.fontColor(TS_8b_Green,TS_8b_Black);
    display.print(anti_theft);
  
}

void OFF(char* anti_theft){
    default_display();
}


void b(int blink)
{
  while(blink)
  {
    blinkAlert();
  }
}

void back(){
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0,48);
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.print("< Back");
}
