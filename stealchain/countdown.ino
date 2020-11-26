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

    bool userHasNotLeftMyApp = true;


//The primary function of this sub module. This function will return when the user leaves the timer app.
int countdown()
{
    display.setFont(liberationSans_8ptFontInfo);
    display.fontColor(YELLOW, TS_8b_Black);
    display.clearScreen();
    
    displayDefaultMenu();

    while (!display.getButtons(TSButtonLowerLeft))
    {
        if (display.getButtons(TSButtonLowerRight) && !hasStarted && (seconds > 0 || minutes > 0 || hours > 0))
        {
            //Timer has been set and user started it
            delay(300);
            updateTimeDisplay();
        }
        else if (display.getButtons(TSButtonLowerRight) && isPaused && hasStarted)
        {
            //Continue button is triggered
            delay(300);
            display.clearWindow(0, 48, 64, 11);
            display.setCursor(56, 48);
            display.print("Pause >");

            display.setCursor(0, 50);
            display.print("< Home");
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
                display.clearWindow(40, 48, 24, 11);
            }
        }
        else if (display.getButtons(TSButtonLowerLeft))
        {
            userHasNotLeftMyApp = false;
        }
    }
    delay(300);
    return 0;
}

//This function calculates the time remaining till the timer ends and prints the timer on the screen.
    void updateTimeDisplay()
    {
        totalSeconds = hours * 3600 + minutes * 60 + seconds;

        char displayX;

        display.clearWindow(56, 48, 40, 16);
        display.setCursor(56, 48);
        display.setFont(liberationSansNarrow_8ptFontInfo);
        display.fontColor(YELLOW, BLACK);
        display.print("Pause >");

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

            display.fontColor(TS_8b_White, TS_8b_Black);
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
            else if (display.getButtons(TSButtonLowerRight) && hasStarted)
            {
                hours = hoursToDisplay;
                minutes = minutesToDisplay;
                seconds = secondsToDisplay;
                isPaused = true;
                display.clearWindow(0, 48, 64, 11);
                display.setFont(liberationSansNarrow_8ptFontInfo);
                display.fontColor(YELLOW, BLACK);
                display.setCursor(45, 48);
                display.print("Continue >");

                display.setCursor(0, 50);
                display.print("< Home");
                delay(300);
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
            else if (display.getButtons(TSButtonLowerLeft))
            {
              userHasNotLeftMyApp = false;
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

            int i = 6;
            unsigned long pretime = 0;
            int shown = 0;

            while (i > 0)
            {
                if (millis() >= pretime + 1000)
                {
                    pretime = millis();
                    SerialMonitorInterface.println("Clear screen");

                    if (shown)
                    {
                        display.clearScreen();
                        shown = 0;
                    }
                    else
                    {
                        shown = 1;
                        display.drawRect(0, 0, 95, 63, 1, TS_8b_Red);
                        display.setCursor(16, 20); //set the cursor to x, y position, where to start next print/draw instruction
                        display.setFont(liberationSansNarrow_12ptFontInfo);
                        display.fontColor(TS_8b_Black, TS_8b_Red);
                        display.print("WAKE UP!!");
                    }
                    i--;
                }
            }
            displayDefaultMenu();
        }
    }

// When this function is called, the user will be able to adjust the values of the timer such as the hours, minutes and seconds left.
    void adjustTimer()
    {
        //Remove pause/continue button from Display
        display.clearWindow(0, 48, 64, 11);

        char displayX;

        display.clearWindow(0, 0, 96, 11);
        display.setFont(liberationSansNarrow_8ptFontInfo);
        display.fontColor(YELLOW, BLACK);
        display.setCursor(0, 0);
        delay(200);
        display.print("< Next");

        display.setFont(liberationSansNarrow_8ptFontInfo);
        display.fontColor(YELLOW, BLACK);
        display.setCursor(90, 0);
        delay(200);
        display.print("^");

        display.setCursor(90, 48);
        display.setFont(liberationSansNarrow_8ptFontInfo);
        display.fontColor(YELLOW, BLACK);
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
                        if (minutes == 0)
                        {
                          minutes = 59;
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
                    display.clearWindow(0, 0, 60, 11);
                    display.setFont(liberationSansNarrow_8ptFontInfo);
                    display.fontColor(YELLOW, BLACK);
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
                    if(seconds == 0)
                    {
                      seconds = 59;
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
                display.clearWindow(0, 0, 96, 11);
                display.clearWindow(0, 48, 96, 11);
                time_now = millis();
                displayDefaultMenu();
                isInSettings = false;

                //Print the start button
                display.setFont(liberationSansNarrow_8ptFontInfo);
                display.fontColor(YELLOW, BLACK);
                display.setCursor(56, 48);
                display.print("Start >");
                break;
            }
            //    delay(200);
        }
    }

// When this function is called, the time remaining is retrieved and printed onto the screen.
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

// When this function is called, the base navigation buttons are displayed onto the screen.
    void displayDefaultMenu()
    {
        display.setFont(liberationSansNarrow_8ptFontInfo);
        display.fontColor(YELLOW, BLACK);
        display.setCursor(0, 0);
        display.print("< Reset");
        display.setCursor(0, 50);
        display.print("< Home");
        display.setCursor(66, 0);
        display.print("Set >");
    }
