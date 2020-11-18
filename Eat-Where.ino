#include <SPI.h>
#include <Wire.h>
#include <TinyScreen.h>
#include <string.h>

TinyScreen display = TinyScreen(TinyScreenDefault);
char *canteens[5] = {"North Canteen", "South Canteen", "Koufu", "Foogle Hub", "Food Connect"};
char *north_canteen[5] = {"Western Cuisine", "Pasta (Halal)", "Economical Rice", "Thai Cuisine", "Indo Delight (Muslim Operated)"};
char *south_canteen[8] = {"Muslim Food (Muslim Operated)", "Vegetarian", "Chicken Rice", "Indian Food", "Japanese Food", "Ban Mian & Fish Soup", "Economy Rice (Muslim Operated)", "Western Food (Muslim Operated)"};
char *koufu[11] = {"Fish Ball Noodle", "Yong Tau Foo", "Thai Cuisine (Muslim Operated)", "Korean Cuisine", "Nasi Padang (Muslim Operated)", "Curry Rice", "Western", "Japanese Cuisine", "Vegetarian", "Turkish Kebab", "Mala Hot Pot"};
char *foogle[7] = {"Fusion Food", "Ayam Penyet (Muslim Operated)", "Japanese/Korean", "Thai Cuisine", "Chicken Rice (Halal)", "Economy Rice & Mala Hotpot", "Indian Muslim Cuisine (Muslim Operated)"};
char *food_connect[3] = {"Manna (Western Cuisine/Pastry/Ice-cream) (Halal)", "Subway (Sandwiches/Salad) (Halal)", "International Mart (Mala Hot Pot/Lok Lok/Salad/Waffles)"};

int choose_place = 0;
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
  display.setFont(liberationSansNarrow_12ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
  display.clearScreen();
  display.setCursor(5, 25);
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
    displayMenu2();
    chooseStall();
  }
}

void choosePlace()
{
  if (display.getButtons(TSButtonUpperLeft)) {
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
  if (display.getButtons(TSButtonUpperLeft)) {
    
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
  display.print("Choose");
  display.setCursor(0, 48);
  display.print("Back");
}

void displayMenu2()
{
  display.setCursor(0, 0);
  display.print("Choose Stall");
}
