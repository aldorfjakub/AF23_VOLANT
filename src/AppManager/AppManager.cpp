#include "AppManager.h"

void AppManager::setApp(App *app)
{
  
  currentApp = app;
  currentApp->init();
}

void AppManager::update()
{
  if (currentApp)
  {
    currentApp->update();
  }
}
