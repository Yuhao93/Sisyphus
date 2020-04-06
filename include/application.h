#ifndef APPLICATION_H
#define APPLICATION_H

#include "led.h"
#include "pattern_manager.h"

class Application {
public:
  Application() {}
  
  PatternManager pattern_manager;
  Led led;
};

#endif
