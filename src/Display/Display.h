#ifndef DISPLAY_H
#define DISPLAY_H
#include <TFT_eSPI.h>
#include <Arduino.h>

struct region
{
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
};

enum TouchState
{
  UNPRESSED,
  PRESSED,
  HOLDING
};

class Display
{
public:
  void init();

  void update();
  void setBrightness(uint8_t brightness);
  bool isRegionTouched(region re);
  void Displaycalibrate();
  void drawBattery(u16_t x, u16_t y, u16_t id, u16_t percentage,float voltage );
  void drawBootLogo();
  TouchState getTouchState();
  TFT_eSPI tft = TFT_eSPI();
  TFT_eSprite sprite = TFT_eSprite(&tft);

private:
  u16_t touch_x = -1, touch_y = -1;
  u16_t prev_touch_x = -1, prev_touch_y = -1;
  bool isCurrentlyTouched = false;
  TouchState currentTouchState = UNPRESSED;
};

#endif
