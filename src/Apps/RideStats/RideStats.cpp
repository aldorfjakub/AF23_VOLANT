#include "RideStats.h"

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

const float DISTANCE_UPDATE_INTERVAL_s = 1; // Number of seconds needed to pass before the distance driven variable is updated
const int HALL_PULSES_PER_ROTATION = 4;     // Number of pulses detected by all hall sensors on one rotation of the motor
const float BELT_GEAR_RATIO = 0.5;          // Gear ratio of the motor belt - motor rotations : diff rotations
const float DIFF_GEAR_RATIO = 0.25;         // Gear ratio of the differential - diff rotations : wheel rotations
const int WHEEL_DIAMETER_mm = 500;          // Diameter of the back wheels in milimeters
// DO NOT CHANGE
const float WHEEL_CIRCUMFERENCE_mm = WHEEL_DIAMETER_mm * PI; 

void RideStatsApp::init()
{
    serialLinkDriver.sendPacket(0x09,nullptr,0);
}
u32_t last_update_ride = 0;

void RideStatsApp::update()
{
    

    float accel_times[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

       

    if(millis()-last_update_ride>1000){
        serialLinkDriver.sendPacket(0x09,nullptr,0);
        last_update_ride = millis();
        }
    
        float diff_rpm = (float)serialLinkDriver.rideStatsData.max_rpm * BELT_GEAR_RATIO;
        // 2. Convert RPM from the differential to the wheel axes
        float wheel_rpm = (float)diff_rpm * DIFF_GEAR_RATIO;
        // 3. Calculate m/s and km/h
        float speed_mps = wheel_rpm * (WHEEL_CIRCUMFERENCE_mm / 1000.0) / 60.0;
        float speed_kmph = speed_mps * 3.6;

    drawStats(serialLinkDriver.rideStatsData.trip_distance_km, serialLinkDriver.rideStatsData.total_distance_km, serialLinkDriver.rideStatsData.max_rpm, speed_kmph, serialLinkDriver.rideStatsData.best_accel_times);
Serial.print(serialLinkDriver.rideStatsData.trip_distance_km);

}

void RideStatsApp::drawScreen()
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);

    display.sprite.pushSprite(0, 0);
}

void RideStatsApp::drawStats(float distance_driven_trip_m, float distance_driven_total_m, int max_rpm, float max_speed_kmph, float accel_times[])
{
    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    display.sprite.setTextFont(0);
    display.sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    display.sprite.setTextDatum(TL_DATUM);

    display.sprite.setTextSize(1);
    display.sprite.setTextColor(TFT_CYAN);
    display.sprite.drawString("VEHICLE STATISTICS", 10, 5);
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.drawFastHLine(0, 20, 320, TFT_CYAN);

    String str;
    display.sprite.setTextColor(TFT_YELLOW);
    if (distance_driven_trip_m > 1000)
    {
        str = String(distance_driven_trip_m / 1000, 1) + " km";
    }
    else
    {
        str = String(distance_driven_trip_m) + " m";
    }
    display.sprite.drawString("Trip distance:", 10, 30);
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.drawString(str, 100, 30);

    display.sprite.setTextColor(TFT_YELLOW);
    if (distance_driven_total_m > 1000)
    {
        str = String(distance_driven_total_m / 1000, 1) + " km";
    }
    else
    {
        str = String(distance_driven_total_m) + " m";
    }
    display.sprite.drawString("Total distance:", 10, 50);
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.drawString(str, 100, 50);

    display.sprite.setTextColor(TFT_YELLOW);
    display.sprite.drawString("Max RPM:", 160, 30);
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.drawString(String(max_rpm), 225, 30);

    display.sprite.setTextColor(TFT_YELLOW);
    display.sprite.drawString("Max speed:", 160, 50);
    display.sprite.setTextColor(TFT_WHITE);
    display.sprite.drawString(String(max_speed_kmph) + " km/h", 225, 50);

    display.sprite.drawFastHLine(0, 70, 320, TFT_CYAN);

    display.sprite.setTextColor(TFT_CYAN);
    display.sprite.drawString("ACCELERATION TIMES", 10, 80);

    int leftX = 10;
    int rightX = 160;
    int startY = 100;
    int rowSpacing = 14; 

    // First column (0-5 to 0-25)
    for (int i = 0; i < 5; i++)
    {
        int speed = (i + 1) * 10;
        display.sprite.setTextColor(TFT_YELLOW);
        display.sprite.drawString("0-" + String(speed) + " km/h:", leftX, startY + i * rowSpacing);
        display.sprite.setTextColor(TFT_WHITE);
        display.sprite.drawString(String(accel_times[i]) + " s", leftX + 90, startY + i * rowSpacing);
    }

 /*   // Second column (0-30 to 0-50)
    for (int i = 5; i < 10; i++)
    {
        int speed = (i + 1) * 5;
        display.sprite.setTextColor(TFT_YELLOW);
        display.sprite.drawString("0-" + String(speed) + " km/h:", rightX, startY + (i - 5) * rowSpacing);
        display.sprite.setTextColor(TFT_WHITE);
        display.sprite.drawString(String(accel_times[i]) + " s", rightX + 90, startY + (i - 5) * rowSpacing);
    }
*/
    // Push to display
    display.sprite.pushSprite(0, 0);
}
