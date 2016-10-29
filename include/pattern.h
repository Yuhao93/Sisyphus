#ifndef PATTERN_H
#define PATTERN_H

#include "servo_motor.h"
#include "stepper_motor.h"
#include "structs.h"
#include <mutex>
#include <vector>

class Pattern {
public:
  Pattern();
  /**
   * Adds a pattern into the end of the circular buffer of patterns.
   */
  void queuePattern(std::vector<ArmAngle> pattern);

  void step();

private:
  ServoMotor servo_motor;
  StepperMotor stepper_motor;
  std::vector<std::vector<ArmAngle>> patterns;
  std::vector<ArmAngle> current_pattern;
  std::mutex lock;
  int current_index;
  int current_pattern_index;
};

#endif
