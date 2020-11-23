uint8_t defaultFontColor = TS_8b_White;
uint8_t defaultFontBG = TS_8b_Black;
uint8_t inactiveFontColor = TS_8b_Gray;
uint8_t inactiveFontBG = TS_8b_Black;
const FONT_INFO &clockFont = liberationSansNarrow_22ptFontInfo;
uint8_t timeY = 20;

int hours = 0;
int minutes = 0;
int seconds = 0;
int totalSeconds = 0;
int timeInterval = 1000;
unsigned long time_now = 0;

bool isPaused = false;
bool isInSettings = false;
bool timeIsZero = false;
bool hasStarted = false;

bool userHasNotLeftMyApp = false;

display.setFont(liberationSans_8ptFontInfo);
display.fontColor(TS_8b_White, TS_8b_Black);
display.clearScreen();
display.setCursor(5, 25);

displayDefaultMenu();

while (userHasNotLeftMyApp)
{
    if (display.getButtons(TSButtonLowerLeft) && !hasStarted && (seconds > 0 || minutes > 0 || hours > 0))
    {
        //Timer has been set and user started it
        updateTimeDisplay();
    }
    else if (display.getButtons(TSButtonLowerLeft) && isPaused && hasStarted)
    {
        //Continue button is triggered
        delay(200);
        display.clearWindow(0, 48, 64, 11);
        display.setCursor(0, 48);
        display.print("< Pause");
        updateTimeDisplay();
    }
    else if (display.getButtons(TSButtonUpperRight))
    {
        //Set button is triggered
        isInSettings = true;
        adjustTimer();
    }
    else if (display.getButtons(TSButtonUpperLeft && !isInSettings))
    {
        //Reset timer..
        hours = 0;
        minutes = 0;
        seconds = 0;
        totalSeconds = 0;

        hasStarted = false;

        char displayX;

        displayClockAsIs();
    }
    else if (!hasStarted)
    {
        displayClockAsIs();
        if (seconds < 1 && minutes < 1 && hours < 1)
        {
            //Dont show any pause/continue button.
            display.clearWindow(0, 48, 64, 11);
        }
        else
        {
            //      display.clearWindow(0, 48, 64, 11);
            //      display.setFont(liberationSans_8ptFontInfo);
            //      display.fontColor(TS_8b_White, TS_8b_Black);
            //      display.setCursor(0, 48);
            //      display.print("< Start");
        }
    }
}

void updateTimeDisplay()
{
    totalSeconds = hours * 3600 + minutes * 60 + seconds;

    char displayX;

    display.clearWindow(0, 48, 64, 11);
    display.setCursor(0, 48);
    display.setFont(liberationSans_8ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.print("< Pause");

    isPaused = false;

    int internalSeconds = 0;
    int secondsToDisplay = 0;
    int minutesToDisplay = 0;
    int hoursToDisplay = 0;

    while (totalSeconds >= 0)
    {
        hasStarted = true;
        if (millis() >= time_now + timeInterval)
        {
            //      time_now += timeInterval;
            time_now = millis();

            totalSeconds--;
        }
        //Run other code

        internalSeconds = totalSeconds;

        if (internalSeconds >= 3600)
        {
            hoursToDisplay = internalSeconds / 3600;
            //      internalSeconds += internalSeconds % 3600;
            internalSeconds -= hoursToDisplay * 3600;
        }
        if (internalSeconds >= 60)
        {
            minutesToDisplay = internalSeconds / 60;
            //      internalSeconds += internalSeconds % 60;
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

        if (display.getButtons(TSButtonUpperLeft))
        {
            //Reset invoked.
            totalSeconds = 0;
            hours = 0;
            minutes = 0;
            seconds = 0;
            timeIsZero = true;
        }
        else if (display.getButtons(TSButtonLowerLeft) && hasStarted)
        {
            hours = hoursToDisplay;
            minutes = minutesToDisplay;
            seconds = secondsToDisplay;
            isPaused = true;
            display.clearWindow(0, 48, 64, 11);
            display.setFont(liberationSans_8ptFontInfo);
            display.fontColor(TS_8b_White, TS_8b_Black);
            display.setCursor(0, 48);
            display.print("< Continue");
            delay(200);
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
    }

    if (totalSeconds <= 0)
    {
        display.clearWindow(0, 48, 64, 11);

        display.setFont(clockFont);
        displayX = 0;
        display.setCursor(displayX, timeY);
        display.print("00");
        display.write(':');

        display.setFont(clockFont);
        displayX = 14 + 14 - 1;
        display.setCursor(displayX, timeY);
        display.print("00");
        display.write(':');

        display.setFont(clockFont);
        displayX = 14 + 14 + 14 + 14 - 2;
        display.setCursor(displayX, timeY);
        display.print("00");

        totalSeconds = 0;
        hours = 0;
        minutes = 0;
        seconds = 0;

        hasStarted = false;
    }
}

void adjustTimer()
{
    //Remove pause/continue button from Display
    display.clearWindow(0, 48, 64, 11);

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

    bool saveWordsPrinted = false;

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

            if (!saveWordsPrinted)
            {
                display.clearWindow(0, 0, 70, 11);
                display.setFont(liberationSans_8ptFontInfo);
                display.fontColor(TS_8b_White, TS_8b_Black);
                display.setCursor(0, 0);
                display.print("< Save");
                saveWordsPrinted = true;
            }

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
            //      delay(200);
            display.clearWindow(0, 48, 96, 11);
            time_now = millis();
            displayMenu();
            isInSettings = false;

            //Print the start button
            display.setFont(liberationSans_8ptFontInfo);
            display.fontColor(TS_8b_White, TS_8b_Black);
            display.setCursor(0, 48);
            display.print("< Start");
            break;
        }
        //    delay(200);
    }
}

void displayClockAsIs()
{
    char displayX;

    display.setFont(clockFont);
    display.fontColor(defaultFontColor, defaultFontBG);
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
    display.fontColor(defaultFontColor, defaultFontBG);
    displayX = 14 + 14 + 14 + 14 - 2;
    display.setCursor(displayX, timeY);
    if (seconds < 10)
        display.print('0');
    display.print(seconds);
}

void displayDefaultMenu()
{
    display.setFont(liberationSans_8ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.setCursor(0, 0);
    display.print("< Reset");
    //  display.setCursor(0, 48);
    //  display.print("< Start/Stop");
    display.setCursor(66, 0);
    display.print("Set >");
}