#ifndef STEPPER_MOTORS_H
#define STEPPER_MOTORS_H

#define STEPPER_MOTORS_GPIO_ANGLE_STEP 0
#define STEPPER_MOTORS_GPIO_ANGLE_DIR 2
#define STEPPER_MOTORS_GPIO_FEEDBACK 4
#define STEPPER_MOTORS_GPIO_LINEAR_STEP 5
#define STEPPER_MOTORS_GPIO_LINEAR_DIR 6

#define STEPPER_MOTORS_LINEAR_STEPS_RANGE 100
#define STEPPER_MOTORS_ANGULAR_STEPS_RANGE 100

#include "model.pb.h"

class StepperMotors {
public:
  StepperMotors() {}
  void setup();
  void move_to_start(float magnitude);
  void step(const sisyphus::Step& step);
};

#endif
