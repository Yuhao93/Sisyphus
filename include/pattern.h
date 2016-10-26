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
  void step();

private:
  ServoMotor servo_motor;
  StepperMotor stepper_motor;
  std::vector<std::vector<ArmAngle>> patterns;
  std::vector<ArmAngle> current_pattern;
  int current_index;
  int current_pattern_index;
};

#endif
