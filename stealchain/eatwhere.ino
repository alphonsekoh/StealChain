#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>

#define STALL_YPOS 30
#define PLACE_YPOS 25
#define EAT_WHERE "Where to eat?"
#define EAT_WHAT "What to eat?"

char *canteens[5] = {"North Canteen", "South Canteen", "Koufu", "Foogle Hub", "Food Connect"};
char *north_canteen[5] = {"Western Cuisine", "Pasta", "Economical Rice", "Thai Cuisine", "Indo Delight"};
char *south_canteen[8] = {"Muslim Food", "Vegetarian", "Chicken Rice", "Indian Food", "Japanese Food", "Ban Mian", "Economy Rice", "Western Food"};
char *koufu[11] = {"Fishball Noodle", "Yong Tau Foo", "Thai Cuisine", "Korean Cuisine", "Nasi Padang", "Curry Rice", "Western", "Japanese Cuisine", "Vegetarian", "Turkish Kebab", "Mala Hotpot"};
char *foogle[7] = {"Fusion Food", "Ayam Penyet", "Japanese/Korean", "Thai Cuisine", "Chicken Rice", "Mala Hotpot", "Indian Muslim"};
char *food_connect[3] = {"Manna", "Subway", "International Mart"};

int is_place_chosen = 0;  // 0 means haven't select place, 1 means selected place
int is_back = 1;
char *chosen_place;
long randIndex;

int eatWhere()
{
  eatWhereSetup();

  // Keep looping until lower left button is pressed
  while (!display.getButtons(TSButtonLowerLeft))
  {
    eatWhereLoop();
  }
  
  delay(300);
  if(is_place_chosen)
  {
    display.clearScreen();
    is_place_chosen = 0;
    eatWhere();
  }
  else
  {
    is_place_chosen = 0;
    return 0;
  }
}

void eatWhereSetup() {
  display.clearScreen();
  display.setFont(liberationSans_10ptFontInfo); // Set font size
  display.fontColor(WHITE, BLACK); // Set font color to white
  centerText(EAT_WHERE, PLACE_YPOS); 
  displayMenu();
  
}


void eatWhereLoop() 
{
  if (!is_place_chosen)
  {
    choosePlace();
  }
  else
  { 
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(0, 0);
    display.fontColor(YELLOW, BLACK);
    display.print("< Choose Stall");
    while (!display.getButtons(TSButtonLowerLeft))
    {
      chooseStall();
    }
  }
}

/**
 * 
 * Randomnise and display chosen place to eat
 * Set choose place to 1 to indicate that place
 * has been chosen
 * 
*/
void choosePlace()
{
  if (display.getButtons(TSButtonUpperLeft)) 
  {
    display.fontColor(WHITE,BLACK);
    display.setFont(liberationSans_10ptFontInfo);
    displayRandom(5, canteens, PLACE_YPOS);
    delay(200);
    randIndex = random(5);
    chosen_place = canteens[(int)randIndex];
    display.fontColor(GREEN, BLACK);
    centerText(chosen_place, PLACE_YPOS);
    drawBorder();
    delay(600);
    display.clearWindow(0, PLACE_YPOS, 96, 20);
    centerText(chosen_place, 15);
    
    display.fontColor(WHITE,BLACK);
    centerText(EAT_WHAT, 30);
    
    is_place_chosen = 1;
    display.clearWindow(0, 0, 96, 11);
    display.clearWindow(0,52, 96, 12);
    display.fontColor(YELLOW, BLACK);
    display.setFont(liberationSans_8ptFontInfo);
    display.setCursor(0, 48);
    display.print("< Back");
  }
}

/**
 * 
 * Randomnise and display chosen stall 
 * based on selected place
 * 
*/
void chooseStall()
{
  if (display.getButtons(TSButtonUpperLeft)) 
  {
    display.fontColor(WHITE,BLACK);
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(0, STALL_YPOS, 96, 15);
    switch ((int)randIndex)
    {
      case 0:
      {
        displayRandom(5, north_canteen, STALL_YPOS);
        delay(200);
        centerText(north_canteen[random(5)], STALL_YPOS);
      }
      break;
      case 1:
      {
        displayRandom(8, south_canteen, STALL_YPOS);
        delay(200);
        centerText(south_canteen[random(8)], STALL_YPOS);
      }
      break;
      case 2:
      {
        displayRandom(11, koufu, STALL_YPOS);
        delay(200);
        centerText(koufu[random(11)], STALL_YPOS);
      }
      break;
      case 3:
      {
        displayRandom(7, foogle, STALL_YPOS);
        delay(200);
        centerText(foogle[random(7)], STALL_YPOS);
      }
      break;
      case 4:
      {
        displayRandom(3, food_connect, STALL_YPOS);
        delay(200);
        centerText(food_connect[random(3)], STALL_YPOS);
      }
      break;
    }
    drawBorder();
  }
}

/**
 * 
 * Display default menu items
 * 
*/
void displayMenu()
{ 
  display.clearWindow(0, 0, 96, 12);
  display.fontColor(YELLOW,BLACK);
  display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0, 0);
  display.print("< Choose Place");
  display.setCursor(0, 48);
  display.print("< Home");
}

/**
 * 
 * Display text randomly for 10 times before 
 * showing the chosen place or stall
 * 
*/
void displayRandom(int num, char ** list, int height)
{
  for (int i = 0; i < 10; i++)
  {
    delay(100);
    display.clearWindow(0, height, 96, 15);
    delay(100);
    centerText(list[random(num)], height);
  }
  delay(100);
  display.clearWindow(0, height, 96, 15);
}

/**
 * 
 * Check if lower left button is pressed
 * and reset display to default screen
 * 
*/
void backButtonLoop()
{
  if (display.getButtons(TSButtonLowerLeft))
  {
    delay(400);
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(0, 0, 96, 44);
    centerText(EAT_WHERE, 25);
    displayMenu();
    is_place_chosen = 0;
  }
}

/**
 * 
 * Calculate width based on text size
 * and display text to center
 * 
*/
void centerText(char *txt, int ypos)
{
  int width=display.getPrintWidth(txt);
  SerialMonitorInterface.println(width);
  if(width <= 96)
  {
    display.setCursor(48-(width/2), ypos);
  }
  else
  {
    display.setCursor(0, ypos);
  }
  display.print(txt);
}

/**
 * 
 * Draw an animated border starting from top left
 * and clears the border after completing a round
*/
void drawBorder()
{
  // Draw top border
  for(int i=1; i<=96; i++)
  {
    display.drawLine(0,0, i, 0, TS_8b_Green);
  }
  // Draw right border
  for(int i=1; i<=64; i++)
  {
    display.drawLine(96,0, 96, i, TS_8b_Green);
  }
  // Draw bottom border
  for(int i=96; i>=0; i--)
  {
    display.drawLine(96,64, i, 64, TS_8b_Green);
  }
  // Draw left border
  for(int i=64; i>=0; i--)
  {
    display.drawLine(0,64, 0, i, TS_8b_Green);
  }

  // Clear top border
  for(int i=1; i<=96; i++)
  {
    display.drawLine(0,0, i, 0, TS_8b_Black);
  }
  // Clear right border
  for(int i=1; i<=64; i++)
  {
    display.drawLine(96,0, 96, i, TS_8b_Black);
  }
  // Clear bottom border
  for(int i=96; i>=0; i--)
  {
    display.drawLine(96,64, i, 64, TS_8b_Black);
  }
  // Clear left border
  for(int i=64; i>=0; i--)
  {
    display.drawLine(0,64, 0, i, TS_8b_Black);
  }
}
