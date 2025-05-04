#ifndef MENU_APP_H
#define MENU_APP_H

#include "./Apps/app.h"
#include <Arduino.h>
#include "./Display/Display.h"
#include "./Apps/MainScreen/MainScreen.h"
#include "./Apps/Battery/BatteryInfo.h"
#include "./Apps/Timer/TimerApp.h"
#include "./Apps/BlockGame/BlockGame.h"
#include "./Apps/TempApp/TempApp.h"
#include "./Apps/RideStats/RideStats.h"








extern MainApp mainApp;
extern BatteryInfo  batteryInfo;
extern TimerApp timerApp;
extern BlockGameApp blockGameApp;
extern TempApp tempApp;
extern RideStatsApp rideStats;


class Part
{
public:
  int id;
  String name;
 
  App* app;

  Part(int set_id, String set_name,  App* set_app)
  {
    id = set_id;
    name = set_name;
    
    app = set_app;
  }
};

class MenuApp : public App
{
private:
  void drawMenu();
  void drawPart(u16_t x, u16_t y, bool active, Part part);
  int activeItem;
  int startEncoder;
  Part parts[8] = {Part(0, "Dashboard", &mainApp), Part(1, "Battery",&batteryInfo), Part(2, "Timer", &timerApp),Part(3, "BlockGame", &blockGameApp),Part(4, "Temperature", &tempApp),Part(5, "Ride stats", &rideStats),Part(6, "Settings", &mainApp),Part(7, "Reboot", &mainApp)};

public:
  void init() override;
  void update() override;
};

#endif