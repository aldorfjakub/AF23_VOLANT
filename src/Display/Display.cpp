#include "Display.h"
#include <TFT_eSPI.h>
#include <images/logo1.h>


void Display::init()
{
    tft.init();
    tft.setRotation(3);
    tft.invertDisplay(1);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 1);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.setTextFont(1);

    uint16_t calibration_data[] = {459, 3370, 467, 3040, 5};

    tft.setTouch(calibration_data);

    sprite.setColorDepth(16);

    sprite.createSprite(320, 240);



    Display::setBrightness(255);
}

bool Display::isRegionTouched(region re)
{
    if (this->touch_x < re.x || this->touch_x > re.x + re.w)
        return false;
    if (this->touch_y < re.y || this->touch_y > re.y + re.h)
        return false;
    return true;
}

void Display::setBrightness(uint8_t brightness)
{
    analogWrite(TFT_BL, brightness);
}
void Display::update()
{
    prev_touch_x = touch_x;
    prev_touch_y = touch_y;

    if (this->tft.getTouch(&this->touch_x, &this->touch_y) == 0)
    {
        this->touch_x = -1;
        this->touch_y = -1;
    }
    TouchState state = getTouchState();
    
    switch (state)
    {
    case PRESSED:
       // Serial.println("PRESSED");
        break;
    case HOLDING:
        //Serial.println("HOLDING");
        break;
    case UNPRESSED:
       // Serial.println("UNPRESSED");
        break;
    }


}
void Display::drawBootLogo()
    {
        sprite.fillSprite(TFT_BLACK);
        sprite.pushImage((320 - LOGO_WIDTH) / 2, (240 - LOGO_HEIGHT) / 2, LOGO_WIDTH, LOGO_HEIGHT, Logo);
        sprite.pushSprite(0, 0);
    }

TouchState Display::getTouchState()
{
    bool wasPreviouslyTouched = (prev_touch_x != 65535 && prev_touch_y != 65535);
    isCurrentlyTouched = (touch_x != 65535 && touch_y != 65535);

    if (isCurrentlyTouched)
    {
        if (wasPreviouslyTouched)
        {
            currentTouchState = HOLDING;
        }
        else
        {
            currentTouchState = PRESSED;
        }
    }
    else
    {
        currentTouchState = UNPRESSED;
    }

    return currentTouchState;
}

void Display::Displaycalibrate()
{
    // Fill the screen with a different color
    tft.fillScreen(TFT_DARKGREEN);

    // Do the calibration
    uint16_t calibration_data[5];
    tft.calibrateTouch(calibration_data, TFT_WHITE, TFT_DARKGREEN, 32);
    tft.setTouch(calibration_data);

    // Show the calibration data on the screen
    tft.drawString("Calibration data:", 0, 0, 2);
    tft.drawNumber(calibration_data[0], 0, 16, 2);
    tft.drawNumber(calibration_data[1], 0, 32, 2);
    tft.drawNumber(calibration_data[2], 0, 48, 2);
    tft.drawNumber(calibration_data[3], 0, 64, 2);
    tft.drawNumber(calibration_data[4], 0, 80, 2);
    delay(10000);
}

void Display::drawBattery(u16_t x, u16_t y, u16_t id, u16_t percentage, float voltage)
{

    this->sprite.setTextDatum(TC_DATUM);

    this->sprite.drawString(String(id), x + 11, y);

    // --- Battery ---
    this->sprite.fillRect(x + 6, y + 12, 8, 4, TFT_WHITE); // Top thing

    this->sprite.fillRect(x, y + 16, 20, 54, TFT_WHITE);     // White part of the battery
    this->sprite.fillRect(x + 2, y + 18, 16, 50, TFT_BLACK); // Empty part
    uint8_t chunksActive = ceil(percentage / 25.);
    int chunkColor = 0x9f2a;
    int chunkTopColor = 0x6de6;
    if (chunksActive < 4)
    {
        chunkColor = 0xfc06;
        chunkTopColor = 0xfdcf;
    }
    for (uint8_t c = 0; c < chunksActive; c++)
    {
        this->sprite.fillRect(x + 2, y + 58 - c * 10, 16, 10, chunkColor);
        this->sprite.fillRect(x + 2, y + 58 - c * 10, 16, 2, chunkTopColor);
    }
    this->sprite.setTextFont(0);
    this->sprite.setTextSize(0);

    String v = String(voltage, 3);
    this->sprite.drawString(v.substring(0,2), x + 10, y + 73);
    this->sprite.drawString(v.substring(2) + "V", x + 10, y + 84);



  
    this->sprite.setTextColor(TFT_WHITE);
}