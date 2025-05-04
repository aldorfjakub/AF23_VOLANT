#include "BatteryInfo.h"

#include "./Display/Display.h"
#include "./InputHandler/InputHandler.h"
#include "./AppManager/AppManager.h"
#include "./Apps/MenuApp/MenuApp.h"
#include "./SerialLinkDriver/SerialLinkDriver.h"

extern Display display;
extern InputHandler inputHandler;
extern AppManager appManager;
extern SerialLinkDriver serialLinkDriver;

extern MenuApp menuApp;

void BatteryInfo::init()
{
    serialLinkDriver.getBatteryData();
}
u32_t last_update_bat = 0;

void BatteryInfo::update()
{
    if ((display.isRegionTouched({0, 0, 320, 240}) && display.getTouchState() == PRESSED) || inputHandler.getButtonState(LEFT_WHITE_BUTTON) == BUTTON_PRESSED)
    {

        appManager.setApp(&menuApp);
    }
   

    if(millis()-last_update_bat>1000){
        serialLinkDriver.getDashboardData();
    last_update_bat = millis();
        }
    
    drawScreen();
}

void BatteryInfo::drawScreen()
{

    display.sprite.fillRect(0, 0, 320, 240, TFT_BLACK);
    display.sprite.setTextFont(0);
    display.sprite.setTextSize(2);

    display.sprite.setTextColor(TFT_YELLOW);
    display.sprite.setTextDatum(CC_DATUM);

    display.sprite.drawString(String(serialLinkDriver.batteryData.totalVoltage / 1000.f) + " V", 60, 20);
    display.sprite.drawString(String(serialLinkDriver.batteryData.current / 1000.f) + " A", 160, 20);
    display.sprite.drawString(String(serialLinkDriver.batteryData.rsoc) + " %", 260, 20);

    display.sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    display.sprite.setTextDatum(TL_DATUM);
    display.sprite.setTextSize(1);

    uint8_t batteryIndex = 0;

    // 1, 2 ,3
    display.sprite.drawRect(8, 40, 108, 98, 0x667c);
    for (uint8_t i = 0; i < 3; i++)
    {

        display.drawBattery(13 + 39 * i, 44, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }

    display.sprite.drawRect(125, 40, 108, 98, 0x667c);
    for (uint8_t i = 0; i < 3; i++)
    {

        display.drawBattery(130 + 39 * i, 44, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }
    display.sprite.drawRect(242, 40, 108, 98, 0x667c);
    for (uint8_t i = 0; i < 2; i++)
    {

        display.drawBattery(250 + 39 * i, 44, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }
    //78 98
    display.sprite.drawRect(-2, 140, 78, 98, 0x667c);
    for (uint8_t i = 0; i < 2; i++)
    {

        display.drawBattery(5 + 39 * i, 144, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }

    display.sprite.drawRect(86, 140, 108, 98, 0x667c);
    for (uint8_t i = 0; i < 3; i++)
    {

        display.drawBattery(91 + 39 * i, 144, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }
    display.sprite.drawRect(204, 140, 108, 98, 0x667c);
    for (uint8_t i = 0; i < 3; i++)
    {

        display.drawBattery(209 + 39 * i, 144, batteryIndex + 1,
                            serialLinkDriver.batteryData.rsoc,                                 // Percentage
                            serialLinkDriver.batteryData.cellVoltages[batteryIndex] / 1000.f); // Voltage
        batteryIndex++;
    }
    /*
        // Top left
        display.sprite.drawRect(8, 8, 148, 110, 0x667c);


        // Top center
        display.sprite.drawRect(164, 8, 70, 110, 0x667c);
        for (uint8_t i = 0; i < 2; i++)
        {
            uint8_t cellIndex = batteryIndex * 3;
            display.drawBattery(169 + 39 * i, 14, batteryIndex + 1,
                                serialLinkDriver.batteryData[cellIndex],             // Voltage
                                serialLinkDriver.batteryData[cellIndex + 1] / 10.0f, // Temperature
                                serialLinkDriver.batteryData[cellIndex + 2]);        // Status
            batteryIndex++;
        }

        // Top right
        display.sprite.drawRect(242, 8, 70, 110, 0x667c);
        for (uint8_t i = 0; i < 2; i++)
        {
            uint8_t cellIndex = batteryIndex * 3;
            display.drawBattery(247 + 39 * i, 14, batteryIndex + 1,
                                serialLinkDriver.batteryData[cellIndex],             // Voltage
                                serialLinkDriver.batteryData[cellIndex + 1] / 10.0f, // Temperature
                                serialLinkDriver.batteryData[cellIndex + 2]);        // Status
            batteryIndex++;
        }
            */

    display.sprite.pushSprite(0, 0);
}
