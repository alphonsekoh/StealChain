#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>

#define STALL_YPOS 30
#define PLACE_YPOS 25
#define WHERE_TO_EAT_TXT "Where to eat?"

TinyScreen display = TinyScreen(TinyScreenDefault);

char *canteens[5] = {"North Canteen", "South Canteen", "Koufu", "Foogle Hub", "Food Connect"};
char *north_canteen[5] = {"Western Cuisine", "Pasta", "Economical Rice", "Thai Cuisine", "Indo Delight"};
char *south_canteen[8] = {"Muslim Food", "Vegetarian", "Chicken Rice", "Indian Food", "Japanese Food", "Ban Mian", "Economy Rice", "Western Food"};
char *koufu[11] = {"Fishball Noodle", "Yong Tau Foo", "Thai Cuisine", "Korean Cuisine", "Nasi Padang", "Curry Rice", "Western", "Japanese Cuisine", "Vegetarian", "Turkish Kebab", "Mala Hotpot"};
char *foogle[7] = {"Fusion Food", "Ayam Penyet", "Japanese/Korean", "Thai Cuisine", "Chicken Rice", "Mala Hotpot", "Indian Muslim"};
char *food_connect[3] = {"Manna", "Subway", "International Mart"};

int choose_place = 0;
char *chosen_place;
long randIndex;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

void setup() {
  Wire.begin(); //initialize I2C before we can initialize TinyScreen
  SerialMonitorInterface.begin(9600);
  display.begin();
  display.on();
  display.setBrightness(10); //sets main current level, valid levels are 0-15
  display.setFlip(true);
  display.setFont(liberationSans_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  centerText(WHERE_TO_EAT_TXT, PLACE_YPOS);
  displayMenu();
}


void loop() {
  if (choose_place == 0)
  {
    choosePlace();
  }
  else
  { 
    display.setFont(liberationSansNarrow_8ptFontInfo);
    display.setCursor(0, 0);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.print("< Choose Stall");
    chooseStall();
  }
  backButtonLoop();
}


void choosePlace()
{
  if (display.getButtons(TSButtonUpperLeft)) 
  {
    display.setFont(liberationSans_10ptFontInfo);
    displayRandom(5, canteens, PLACE_YPOS);
    
    randIndex = random(5);
    chosen_place = canteens[(int)randIndex];
    display.fontColor(TS_8b_Green, TS_8b_Black);
    centerText(chosen_place, PLACE_YPOS);
    delay(800);
    display.clearWindow(0, PLACE_YPOS, 96, 20);
    centerText(chosen_place, 15);
    
    display.fontColor(TS_8b_White, TS_8b_Black);
    centerText("What to eat?", 30);
    
    choose_place = 1;
    display.clearWindow(0, 0, 96, 11);
  }
}


void chooseStall()
{
  if (display.getButtons(TSButtonUpperLeft)) 
  {
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(0, STALL_YPOS, 96, 15);
    switch ((int)randIndex)
    {
      case 0:
      {
        displayRandom(5, north_canteen, STALL_YPOS);
        centerText(north_canteen[random(5)], STALL_YPOS);
      }
      break;
      case 1:
      {
        displayRandom(8, south_canteen, STALL_YPOS);
        centerText(south_canteen[random(8)], STALL_YPOS);
      }
      break;
      case 2:
      {
        displayRandom(11, koufu, STALL_YPOS);
        centerText(koufu[random(11)], STALL_YPOS);
      }
      break;
      case 3:
      {
        displayRandom(7, foogle, STALL_YPOS);
        centerText(foogle[random(7)], STALL_YPOS);
      }
      break;
      case 4:
      {
        displayRandom(3, food_connect, STALL_YPOS);
        centerText(food_connect[random(3)], STALL_YPOS);
      }
      break;
    }
  }
}


void displayMenu()
{ display.setFont(liberationSansNarrow_8ptFontInfo);
  display.setCursor(0, 0);
  display.print("< Choose Place");
  display.setCursor(0, 48);
  display.print("< Back");
}


void displayRandom(int num, char ** list, int height)
{
  for (int i = 0; i < 10; i++)
  {
    delay(200);
    display.clearWindow(0, height, 96, 15);
    centerText(list[random(num)], height);
  }
  display.clearWindow(0, height, 96, 15);
//  display.setCursor(5, height);
}

void backButtonLoop()
{
  if (display.getButtons(TSButtonLowerLeft))
  {
    delay(400);
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(0, 0, 96, 44);
    centerText(WHERE_TO_EAT_TXT, 25);
    displayMenu();
    choose_place = 0;
  }
}

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
