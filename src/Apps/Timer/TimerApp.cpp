#include "TimerApp.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./AppManager/AppManager.h"
#include "./Apps/MenuApp/MenuApp.h"

extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;

extern MenuApp menuApp;
String formatTime(u32_t milliseconds);

void TimerApp::init()
{
    
}

void TimerApp::lap(){
    if (this->timerState == 1) // LAP
    {
        u32_t lapTime = millis() - this->lapStartTime;
        this->previousLap = lapTime;
        if (lapTime < this->bestLap)
        {
            this->bestLap = lapTime;
        }
        this->lapStartTime = millis();
        lapCounter++;
    }

    if (this->timerState == 2) // RESET
    {
        this->timerState = 0;
        this->startTime = 0;
        this->elapsedTime = 0;
        this->lapStartTime = 0;
        this->lapElapsedTime = 0;

        this->bestLap = 4294967295;
        this->previousLap = 0;

        this->lapCounter = 1;
    }
}

void TimerApp::startStop(){
    if (this->timerState == 0) // Start
    {
        this->timerState = 1;
        this->startTime = millis();
        this->lapStartTime = millis();
    }
    else if (this->timerState == 1) // Pause
    {
        this->elapsedTime = millis() - this->startTime;
        this->lapElapsedTime = millis() - this->lapStartTime;

        this->timerState = 2;
    }
    else if (this->timerState == 2) // Resume
    {
        this->startTime = millis() - this->elapsedTime;
        this->lapStartTime = millis() - this->lapElapsedTime;

        this->timerState = 1;
    }
}

void TimerApp::update()
{

    
    if ((display.isRegionTouched({0, 0, 60, 40}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(LEFT_WHITE_BUTTON) == BUTTON_PRESSED)
    {

        appManager.setApp(&menuApp);
    }

    if (display.isRegionTouched({0, 40, 140, 200}))
    {
        if (display.getTouchState() == PRESSED)
        {

         startStop();
        }
    }

    if (display.isRegionTouched({180, 40, 140, 200}))
    {
        if (display.getTouchState() == PRESSED)
        {
           lap();
        }
    }

    drawScreen();
}

u32_t TimerApp::getLapTime(){
   

    if (startTime != 0)
    {
       
       return( millis() - this->lapStartTime);
    }
    return 0;
}

void TimerApp::drawScreen()
{
    if (this->timerState == 2)
    {
        return;
    }

    u32_t time = 0;
    u32_t lapTime = 0;
    if (startTime != 0)
    {
        time = millis() - this->startTime;
        lapTime = millis() - this->lapStartTime;
    }

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    // Header

    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.setTextDatum(TC_DATUM);
    display.sprite.drawString("LAP: " + String(this->lapCounter), 160, 8);
    display.sprite.drawFastHLine(0, 32, 320, TFT_WHITE);

    // Main timer
    display.sprite.setTextColor(TFT_YELLOW, TFT_BLACK);
    display.sprite.setTextDatum(MC_DATUM);
    display.sprite.setTextSize(4);
    display.sprite.drawString(formatTime(lapTime), 160, 85);

    // Subtitle
    display.sprite.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    display.sprite.setTextDatum(MC_DATUM);
    display.sprite.setTextSize(2);
    display.sprite.drawString(formatTime(time), 160, 130);

    // Lap info section
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.setTextSize(2);
    display.sprite.setTextDatum(TL_DATUM);
    display.sprite.drawString("Prev: " + formatTime(this->previousLap), 15, 190);

    display.sprite.setTextColor(TFT_RED);

    if (this->bestLap == 4294967295)
    {
        display.sprite.drawString("Best: " + formatTime(0), 15, 215);
    }
    else
    {
        display.sprite.drawString("Best: " + formatTime(this->bestLap), 15, 215);
    }
    display.sprite.setTextColor(TFT_WHITE);

    display.sprite.pushSprite(0, 0);
}

String formatTime(u32_t milliseconds)
{
    // Convert nanoseconds to milliseconds

    // Extract minutes, seconds, and remaining milliseconds
    u32_t minutes = milliseconds / 60000;
    milliseconds %= 60000;
    u32_t seconds = milliseconds / 1000;
    milliseconds %= 1000;

    // Format the time as a string
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%u:%02u.%03u", (unsigned int)minutes, (unsigned int)seconds, (unsigned int)milliseconds);

    return String(buffer);
}