#ifndef BATTERY_APP_H
#define BATTERY_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class BatteryInfo : public App
{
private:
  void drawScreen();

public:
  void init() override;
  void update() override;
};

#endif