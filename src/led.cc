#include "led.h"

#include "gpio.h"

Led::Led() {}

void Led::setIntensity(double intensity) {
  intensity_ = intensity;
  int value = (int) (GPIO_LED_RANGE * intensity);
  Gpio::pwmWrite(GPIO_LED, value);
}

double Led::getIntensity() {
  return intensity_;
}
