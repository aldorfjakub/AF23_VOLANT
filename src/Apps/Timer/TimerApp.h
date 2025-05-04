#ifndef TIMER_APP_H
#define TIMER_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class TimerApp : public App
{
private:
  void drawScreen();
  u8_t timerState =0; //IDLE 0 | RUNNING 1 | PAUSED 2

  u32_t startTime =0;
  u32_t elapsedTime;
  u32_t lapStartTime;
  u32_t lapElapsedTime;


  u32_t bestLap = 4294967295;
  u32_t previousLap;

  u8_t lapCounter=1;

public:
  void init() override;
  void update() override;

  void lap();
  void startStop();

  u32_t getLapTime();

};

#endif