#ifndef STATS_APP_H
#define STATS_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"

class RideStatsApp : public App
{
private:
  void drawScreen();

public:
  void init() override;
  void update() override;
  void drawStats(float distance_driven_trip_m, float distance_driven_total_m, int max_rpm, float max_speed_kmph, float accel_times[]);
};

#endif