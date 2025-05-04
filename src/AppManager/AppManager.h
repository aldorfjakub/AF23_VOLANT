#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "./Apps/app.h"

class AppManager
{
private:
  App *currentApp;

public:
  void setApp(App *app);
  void update();
  bool menuActive;
};

#endif
