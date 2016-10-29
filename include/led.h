#ifndef LED_H
#define LED_H

#define LED_GPIO_PIN 3
#define LED_INTENSITY_RANGE 100

namespace Led {
  void setup();

  /**
   * An intensity value between 0 and 1. 0 is off, 1 is fully bright.
   */
  void setIntensity(float intensity);
}

#endif
