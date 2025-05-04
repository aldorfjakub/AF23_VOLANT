#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>


class LEDController {
public:
  void init();                          
  void setLeds(uint16_t ledMask);   
                      
  void setLedsByCount(uint8_t count); 
};

#endif
