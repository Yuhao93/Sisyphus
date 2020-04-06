#include "led.h"

#include "wiring_pi_wrapper.h"
#include <softPwm.h>

Led::Led() {
  softPwmCreate(LED_GPIO_PIN, 0, LED_INTENSITY_RANGE);
}

void Led::setIntensity(double intensity) {
  intensity_ = intensity;
  int value = (int) (LED_INTENSITY_RANGE * intensity);
  softPwmWrite(LED_GPIO_PIN, value);
}

double Led::getIntensity() {
  return intensity_;
}
