#include "softPwm.h"

#include "wiringPi.h"
#include <cstdio>

namespace SoftPwmImpl {
  int usPerStep = 100;
  int range;
}

void softPwmCreate(int pin, int initialValue, int range) {
  SoftPwmImpl::range = range;
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: Initialzed pin %d to be PWM with range %d\n",
        pin, range);
  }
}

void softPwmWrite(int pin, int value) {
  if (WIRING_PI_DEBUG) {
    int period = SoftPwmImpl::usPerStep * SoftPwmImpl::range;
    float uptime = (float) period * (float) value / (float) SoftPwmImpl::range;
    printf("[WiringPi DEBUG]: Pwm written with period %d and uptime %f\n",
        period, uptime);
  }
}
