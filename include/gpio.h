#ifndef GPIO_H
#define GPIO_H

#include "wiring_pi_wrapper.h"

#define GPIO_STEPPER_MOTORS_LINEAR_STEP 0
#define GPIO_STEPPER_MOTORS_LINEAR_DIR 2
#define GPIO_STEPPER_MOTORS_LINEAR_MODE_1 12
#define GPIO_STEPPER_MOTORS_LINEAR_MODE_2 13
#define GPIO_STEPPER_MOTORS_LINEAR_MODE_3 14

#define GPIO_STEPPER_MOTORS_ANGLE_STEP 4
#define GPIO_STEPPER_MOTORS_ANGLE_DIR 5
#define GPIO_STEPPER_MOTORS_ANGLE_MODE_1 10
#define GPIO_STEPPER_MOTORS_ANGLE_MODE_2 11
#define GPIO_STEPPER_MOTORS_ANGLE_MODE_3 31

#define GPIO_LINEAR_FEEDBACK 6

#define GPIO_LED 3
#define GPIO_LED_RANGE 100

namespace Gpio {
  void setup();
  void pwmWrite(int pin, int value);
  void write(int pin, int value);
  int read(int pin);
}

#endif
