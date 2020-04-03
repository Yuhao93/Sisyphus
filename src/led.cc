#include "led.h"

#include "wiring_pi_wrapper.h"
#include <softPwm.h>

void Led::Led() {
  softPwmCreate(LED_GPIO_PIN, 0, LED_INTENSITY_RANGE);
}

void Led::setIntensity(float intensity) {
  intensity_ = intensity;
  int value = (int) (LED_INTENSITY_RANGE * intensity);
  softPwmWrite(LED_GPIO_PIN, value);
}

double Led::getIntensity() {
  return intensity_;
}
