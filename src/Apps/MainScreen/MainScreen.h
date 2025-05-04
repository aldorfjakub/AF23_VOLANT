#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class MainApp : public App
{
private:
  void drawScreen(u64_t timer,String m,u16_t mode, u16_t rpm,u16_t speed_kmph, float b_tempeture, u16_t b_percentage, u8_t motorBrake);

public:
  void init() override;
  void update() override;
};

#endif