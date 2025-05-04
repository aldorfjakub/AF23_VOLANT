#ifndef TEMP_APP_H
#define TEMP_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class TempApp : public App
{
private:
  void drawScreen();
  uint16_t getTempColor(float temperature);

public:
  void init() override;
  void update() override;
};

#endif