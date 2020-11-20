#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>

#define STALL_YPOS 30
#define PLACE_YPOS 25

TinyScreen display = TinyScreen(TinyScreenDefault);

char *canteens[5] = {"North Canteen", "South Canteen", "Koufu", "Foogle Hub", "Food Connect"};
char *north_canteen[5] = {"Western Cuisine", "Pasta (Halal)", "Economical Rice", "Thai Cuisine", "Indo Delight (Muslim Operated)"};
char *south_canteen[8] = {"Muslim Food (Muslim Operated)", "Vegetarian", "Chicken Rice", "Indian Food", "Japanese Food", "Ban Mian & Fish Soup", "Economy Rice (Muslim Operated)", "Western Food (Muslim Operated)"};
char *koufu[11] = {"Fish Ball Noodle", "Yong Tau Foo", "Thai Cuisine (Muslim Operated)", "Korean Cuisine", "Nasi Padang (Muslim Operated)", "Curry Rice", "Western", "Japanese Cuisine", "Vegetarian", "Turkish Kebab", "Mala Hot Pot"};
char *foogle[7] = {"Fusion Food", "Ayam Penyet (Muslim Operated)", "Japanese/Korean", "Thai Cuisine", "Chicken Rice (Halal)", "Economy Rice & Mala Hotpot", "Indian Muslim Cuisine (Muslim Operated)"};
char *food_connect[3] = {"Manna (Western Cuisine/Pastry/Ice-cream) (Halal)", "Subway (Sandwiches/Salad) (Halal)", "International Mart (Mala Hot Pot/Lok Lok/Salad/Waffles)"};

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
  display.setCursor(5, PLACE_YPOS);
  display.print("Where to eat?");
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
    display.clearWindow(5, PLACE_YPOS, 96, 20);
    display.setCursor(5, 15);
    display.fontColor(TS_8b_Green, TS_8b_Black);
    display.print((String)"[" + chosen_place + "]");
    
    display.setCursor(5, 30);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.print("What to eat?");
    
    choose_place = 1;
    display.clearWindow(0, 0, 96, 11);
  }
}


void chooseStall()
{
  if (display.getButtons(TSButtonUpperLeft)) 
  {
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(5, STALL_YPOS, 96, 20);
    display.setCursor(5, STALL_YPOS);
    switch ((int)randIndex)
    {
      case 0:
      {
        displayRandom(5, north_canteen, STALL_YPOS);
        display.print(north_canteen[random(5)]);
      }
      break;
      case 1:
      {
        displayRandom(8, south_canteen, STALL_YPOS);
        display.print(south_canteen[random(8)]);
      }
      break;
      case 2:
      {
        displayRandom(11, koufu, STALL_YPOS);
        display.print(koufu[random(11)]);
      }
      break;
      case 3:
      {
        displayRandom(7, foogle, STALL_YPOS);
        display.print(foogle[random(7)]);
      }
      break;
      case 4:
      {
        displayRandom(3, food_connect, STALL_YPOS);
        display.print(food_connect[random(3)]);
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
    display.clearWindow(5, height, 96, 20);
    display.setCursor(5, height);
    display.print(list[random(num)]);
  }
  display.clearWindow(5, height, 96, 20);
  display.setCursor(5, height);
}

void backButtonLoop()
{
  if (display.getButtons(TSButtonLowerLeft))
  {
    delay(400);
    display.setFont(liberationSans_10ptFontInfo);
    display.clearWindow(0, 0, 96, 44);
    display.setCursor(5, 25);
    display.print("Where to eat?");
    displayMenu();
    choose_place = 0;
  }
}
