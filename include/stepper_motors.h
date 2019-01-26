#ifndef STEPPER_MOTORS_H
#define STEPPER_MOTORS_H

#define STEPPER_MOTORS_LINEAR_STEPS_RANGE 100
#define STEPPER_MOTORS_ANGULAR_STEPS_RANGE 100

#include "model.pb.h"

class StepperMotors {
public:
  void step(const sisyphus::Step& step);
};

#endif
