int countdown()
{
//    uint8_t defaultFontColor = TS_8b_White;
//    uint8_t defaultFontBG = TS_8b_Black;
//    uint8_t inactiveFontColor = TS_8b_Gray;
//    uint8_t inactiveFontBG = TS_8b_Black;
//    const FONT_INFO &clockFont = liberationSansNarrow_22ptFontInfo;
//    uint8_t timeY = 20;
//
//    int hours = 0;
//    int minutes = 0;
//    int seconds = 0;
//    int totalSeconds = 0;
//    int timeInterval = 1000;
//    unsigned long time_now = 0;
//
//    bool isPaused = false;
//    bool isInSettings = false;
//    bool timeIsZero = false;
//    bool hasStarted = false;
//
//    bool userHasNotLeftMyApp = false;

    display.setFont(liberationSans_8ptFontInfo);
    display.fontColor(TS_8b_White, TS_8b_Black);
    display.clearScreen();
    display.setCursor(5, 25);

    displayDefaultMenu();

    while (userHasNotLeftMyApp)
    {
        if (display.getButtons(TSButtonLowerRight) && !hasStarted && (seconds > 0 || minutes > 0 || hours > 0))
        {
            //Timer has been set and user started it
            updateTimeDisplay();
        }
        else if (display.getButtons(TSButtonLowerRight) && isPaused && hasStarted)
        {
            //Continue button is triggered
            delay(200);
            display.clearWindow(0, 48, 64, 11);
            display.setCursor(64, 48);
            display.print("Pause >");
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

    



    return 0;
}
