#ifndef PATTERN_H
#define PATTERN_H

#include "servo_motor.h"
#include "stepper_motor.h"
#include "structs.h"
#include <vector>

class Pattern {
public:
  Pattern();
  void queuePattern(std::vector<ArmAngle> pattern);
  void erase();
  void step();

private:
  ServoMotor servo_motor;
  StepperMotor stepper_motor;
  std::vector<ArmAngle> points;
  int current_index;
};

#endif
