#include "LEDController/LEDController.h"
#include "InputHandler/InputHandler.h"
#include "Display/Display.h"
#include "AppManager/AppManager.h"
#include "SerialLinkDriver/SerialLinkDriver.h"
#include <TFT_eSPI.h>


#include "Apps/app.h"
#include "Apps/MenuApp/MenuApp.h"
#include "Apps/MainScreen/MainScreen.h"
#include "Apps/Battery/BatteryInfo.h"
#include "Apps/Timer/TimerApp.h"
#include "Apps/BlockGame/BlockGame.h"
#include "Apps/TempApp/TempApp.h"
#include "Apps/RideStats/RideStats.h"






LEDController ledController;
InputHandler inputHandler;
Display display;
SerialLinkDriver serialLinkDriver;

AppManager appManager;

MenuApp menuApp;
MainApp mainApp;
BatteryInfo batteryInfo;
TimerApp timerApp;
BlockGameApp blockGameApp;
TempApp tempApp;
RideStatsApp rideStats;

void setup()
{
  // Initialize all modules
  Serial.begin(9600);
  delay(2000);
  display.init();

  display.drawBootLogo();
  ledController.init();
  inputHandler.init();
    Serial.println("Setup something!");

  serialLinkDriver.init();
  delay(200);
  // display.Displaycalibrate();
  ledController.setLedsByCount(0);
  appManager.setApp(&menuApp);

   

    Serial.println("Setup finished!");

  serialLinkDriver.getBatteryData();


}

unsigned long lastUpdateTime = 0;
void loop()
{ // TODO! move drives to different core
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 50)
  {
   
    display.update();
    appManager.update();
    serialLinkDriver.update();
    lastUpdateTime = currentTime;
   // Serial.println(inputHandler.getEncoderPosition());
    
    inputHandler.isButtonPressed(BOTTOM_RED_BUTTON);
    inputHandler.update();

  }
}
