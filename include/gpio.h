#ifndef GPIO_H
#define GPIO_H

#define GPIO_STEPPER_MOTORS_ANGLE_STEP 0
#define GPIO_STEPPER_MOTORS_ANGLE_DIR 2
#define GPIO_STEPPER_MOTORS_LINEAR_STEP 5
#define GPIO_STEPPER_MOTORS_LINEAR_DIR 6
#define GPIO_LINEAR_FEEDBACK 4

namespace Gpio {
  void setup();
  void write(int pin, int value);
  int read(int pin);
}

#endif
