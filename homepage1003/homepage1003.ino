bas//Colors available to use
#define BLACK           0x00
#define BLUE            0xE0
#define RED             0x03
#define GREEN           0x1C
#define DGREEN           0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define BROWN           0x32

  
#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include <STBLE.h>

TinyScreen display = TinyScreen(TinyScreenDefault);

void setup(void) {
  Wire.begin();
  display.begin();

}

void loop() {
  displaytime();
}


void displaytime() {
  readInput();
  display.setFont(liberationSans_14ptFontInfo);
  display.fontColor(BLACK,BLACK); //(font_color,background_color)
  display.setCursor(26,20); //Position of clock
  display.fontColor(RED,BLACK); 
  display.print("23:59");
}

void readInput() {
  display.fontColor(TS_8b_White,TS_8b_Black);
  readInput();

}


void buttonLoop() {
  display.setCursor(0, 0); //Position of upper left button
  display.setFont(liberationSans_8ptFontInfo);

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
