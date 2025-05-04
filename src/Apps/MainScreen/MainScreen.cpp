#include "MainScreen.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./LEDController/LEDController.h"

#include "./AppManager/AppManager.h"
#include "./Apps/MenuApp/MenuApp.h"

#include <iostream>
#include <iomanip>

#include "./SerialLinkDriver/SerialLinkDriver.h"

extern SerialLinkDriver serialLinkDriver;
extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;
extern LEDController ledController;

extern TimerApp timerApp;

extern MenuApp menuApp;
// CHANGE TO CONFIGURE THE CALCULATIONS
const float DISTANCE_UPDATE_INTERVAL_s = 1; // Number of seconds needed to pass before the distance driven variable is updated
const int HALL_PULSES_PER_ROTATION = 4;     // Number of pulses detected by all hall sensors on one rotation of the motor
const float BELT_GEAR_RATIO = 0.5;          // Gear ratio of the motor belt - motor rotations : diff rotations
const float DIFF_GEAR_RATIO = 0.25;         // Gear ratio of the differential - diff rotations : wheel rotations
const int WHEEL_DIAMETER_mm = 500;          // Diameter of the back wheels in milimeters
// DO NOT CHANGE
const float WHEEL_CIRCUMFERENCE_mm = WHEEL_DIAMETER_mm * PI; // Circumference of the back wheels in milimetres
String formatTime(uint64_t milliseconds);
void MainApp::init()
{
    serialLinkDriver.getDashboardData();
}

u32_t last_update = 0;

void MainApp::update()
{
    if ((display.isRegionTouched({0, 0, 320, 240}) && display.getTouchState() == PRESSED))
    {

        appManager.setApp(&menuApp);
    }

   

    if(millis()-last_update>1000){
    serialLinkDriver.getDashboardData();
last_update = millis();
    }

    float diff_rpm = (float)serialLinkDriver.dashboardInfo.rpm * BELT_GEAR_RATIO;
    // 2. Convert RPM from the differential to the wheel axes
    float wheel_rpm = (float)diff_rpm * DIFF_GEAR_RATIO;
    // 3. Calculate m/s and km/h
    float speed_mps = wheel_rpm * (WHEEL_CIRCUMFERENCE_mm / 1000.0) / 60.0;
    float speed_kmph = speed_mps * 3.6;
    drawScreen(timerApp.getLapTime(), "M"+(String(serialLinkDriver.dashboardInfo.speedMode+1)), serialLinkDriver.dashboardInfo.rideMode, serialLinkDriver.dashboardInfo.rpm, speed_kmph, serialLinkDriver.dashboardInfo.temperature, serialLinkDriver.dashboardInfo.battery, serialLinkDriver.dashboardInfo.motorBreak);
    ledController.setLedsByCount(15 * (speed_kmph) / 50);
}

void MainApp::drawScreen(u64_t timer, String m, u16_t mode, u16_t rpm, u16_t speed_kmph, float b_tempeture, u16_t b_percentage, u8_t motorBrake)
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    display.sprite.setTextFont(0);

    // --- Top Bar ---
    display.sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    display.sprite.setTextDatum(TL_DATUM);
    display.sprite.setTextSize(2);
    display.sprite.drawFastHLine(0, 32, 320, TFT_WHITE);

    display.sprite.drawString(formatTime(timer), 8, 8);
    u16_t lastXPosition = display.sprite.textWidth(formatTime(timer)) + 8;
    lastXPosition += 8;
    display.sprite.drawFastVLine(lastXPosition, 0, 32, TFT_WHITE);

    display.sprite.setTextColor(TFT_ORANGE);
    lastXPosition += 8;
    display.sprite.drawString(m, lastXPosition, 8);
    lastXPosition += display.sprite.textWidth(m);

    lastXPosition += 8;
    display.sprite.drawFastVLine(lastXPosition, 0, 32, TFT_WHITE);

    // DNR status

    lastXPosition += 8;
    display.sprite.setTextColor(TFT_WHITE);
    if (mode == 0)
    {
        display.sprite.setTextColor(TFT_RED);
    }
    display.sprite.drawString("D", lastXPosition, 8);
    display.sprite.setTextColor(TFT_WHITE);

   
    lastXPosition += 16;
    if (mode == 1)
    {
        display.sprite.setTextColor(TFT_RED);
    }
    display.sprite.drawString("N", lastXPosition, 8);
    display.sprite.setTextColor(TFT_WHITE);

    lastXPosition += 16;

 if (mode == 2)
    {
        display.sprite.setTextColor(TFT_RED);
    }
    display.sprite.drawString("R", lastXPosition, 8);
    lastXPosition += 20;
    display.sprite.drawFastVLine(lastXPosition, 0, 32, TFT_WHITE);

    display.sprite.setTextColor(TFT_RED);
    lastXPosition+=5;
    for(int i =0; i<motorBrake;i++){
        display.sprite.drawString("+", lastXPosition, 8);
    lastXPosition += 12;
    }

    // --- RPM ---
    display.sprite.setTextColor(TFT_YELLOW, TFT_BLACK);
    display.sprite.setTextDatum(TC_DATUM);
    display.sprite.setTextSize(2);
    display.sprite.drawString(String(rpm) + " RPM", 160, 40);

    display.sprite.setTextFont(2);

    // --- Speed ---
    display.sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    display.sprite.setTextSize(5);
    display.sprite.setTextDatum(CC_DATUM);

    display.sprite.drawString(String(speed_kmph), 160, 120); // Speed value
    display.sprite.setTextSize(2);
    display.sprite.setTextDatum(CC_DATUM);

    display.sprite.drawString("km/h", 158, 167);

    // --- Battery ---
    display.sprite.fillRect(14, 166, 8, 4, TFT_WHITE);

    display.sprite.fillRect(8, 170, 20, 54, TFT_WHITE);
    display.sprite.fillRect(10, 172, 16, 50, TFT_BLACK);
    uint8_t chunksActive = 4;
    for (uint8_t i = 0; i < chunksActive; i++)
    {
        display.sprite.fillRect(10, 212 - i * 10, 16, 10, 0x9f2a);
        display.sprite.fillRect(10, 212 - i * 10, 16, 2, 0x6de6);
    }

    display.sprite.setTextDatum(TL_DATUM);
    display.sprite.setTextSize(1);

    display.sprite.drawString(String(b_percentage) + "%", 35, 205);
    display.sprite.setTextColor(0x9f2a);

    display.sprite.drawString(String(b_tempeture) + "°C", 35, 180);

    display.sprite.pushSprite(0, 0);
}

String formatTime(uint64_t milliseconds)
{

    uint64_t minutes = milliseconds / 60000;
    milliseconds %= 60000;
    uint64_t seconds = milliseconds / 1000;
    milliseconds %= 1000;

    // Format the time as a string
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%u:%02u.%03u", (unsigned int)minutes, (unsigned int)seconds, (unsigned int)milliseconds);

    return String(buffer);
}