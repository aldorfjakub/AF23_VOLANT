#include "TempApp.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./LEDController/LEDController.h"

#include "./AppManager/AppManager.h"
#include "./Apps/MenuApp/MenuApp.h"

#include <iostream>
#include <iomanip>
#include "images/formula.h"

#include "./SerialLinkDriver/SerialLinkDriver.h"

extern SerialLinkDriver serialLinkDriver;
extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;
extern LEDController ledController;

extern TimerApp timerApp;

extern MenuApp menuApp;

void TempApp::init()
{
    serialLinkDriver.getTempData();
}
u32_t last_update_temp = 0;

void TempApp::update()
{
       

    if(millis()-last_update_temp>100){
        serialLinkDriver.getTempData();
    last_update_temp = millis();
        }
    

    drawScreen();
}

void TempApp::drawScreen()
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    display.sprite.setTextFont(2);
    display.sprite.pushImage((320 - FORMULA_WIDTH) / 2, (240 - FORMULA_HEIGHT) / 2, FORMULA_WIDTH, FORMULA_HEIGHT, Formula);

    display.sprite.setTextSize(1);
    // Battery 1
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.battery1));
    display.sprite.drawString(String(serialLinkDriver.tempData.battery1/10.f) + " `C", 24, 54);

    // battery 2
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.battery2));
    display.sprite.drawString(String(serialLinkDriver.tempData.battery2/10.f) + " `C", 24, 190);

    // outside
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.outside));
    display.sprite.drawString(String(serialLinkDriver.tempData.outside) + " `C", 214, 29);
    // bms
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.bms));
    display.sprite.drawString(String(serialLinkDriver.tempData.bms/10.f) + " `C", 214, 65);

    // brake
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.brake));
    display.sprite.drawString(String(serialLinkDriver.tempData.brake) + " `C", 214, 102);

    // Motor controller
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.motorController));
    display.sprite.drawString(String(serialLinkDriver.tempData.motorController) + " `C", 214, 142);

    // Control unit
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.controlUnit));
    display.sprite.drawString(String(serialLinkDriver.tempData.controlUnit) + " `C", 214, 180);

    // motor
    display.sprite.setTextColor(getTempColor(serialLinkDriver.tempData.motor));
    display.sprite.drawString(String(serialLinkDriver.tempData.motor) + " `C", 214, 219);

    display.sprite.pushSprite(0, 0);
}
// Function to get temperature color that changes from green (30°C) to red (70°C)
uint16_t TempApp::getTempColor(float temperature)
{
    // Constrain temperature to our gradient range
    if (temperature <= 30.0f)
    {
        return TFT_GREEN; // Full green for temperatures <= 30°C
    }
    else if (temperature >= 70.0f)
    {
        return TFT_RED; // Full red for temperatures >= 70°C
    }
    else
    {
        // Calculate the gradient between green and red
        float percentage = (temperature - 30.0f) / 40.0f; // 40 is the range (70-30)

        // RGB values for green (0,255,0) to red (255,0,0)
        uint8_t red = 255 * percentage;
        uint8_t green = 255 * (1.0f - percentage);

        // Convert RGB to 16-bit color using TFT_eSPI helper method or do it manually
        return display.sprite.color565(red, green, 0);
    }
}
