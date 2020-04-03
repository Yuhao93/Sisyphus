#ifndef GPIO_H
#define GPIO_H

#define GPIO_STEPPER_MOTORS_ANGLE_STEP 0
#define GPIO_STEPPER_MOTORS_ANGLE_DIR 2
#define GPIO_STEPPER_MOTORS_LINEAR_STEP 4
#define GPIO_STEPPER_MOTORS_LINEAR_DIR 5
#define GPIO_LINEAR_FEEDBACK 6

namespace Gpio {
  void setup();
  void write(int pin, int value);
  int read(int pin);
}

#endif
