#include "led.h"

#include <wiringPi.h>
#include <softPwm.h>

void Led::setup() {
  softPwmCreate(LED_GPIO_PIN, 0, LED_INTENSITY_RANGE);
}

void Led::setIntensity(float intensity) {
  int value = (int) (LED_INTENSITY_RANGE * intensity);
  softPwmWrite(LED_GPIO_PIN, value);
}
