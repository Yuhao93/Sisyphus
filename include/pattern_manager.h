#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include "servo_motor.h"
#include "stepper_motor.h"
#include "model.pb.h"
#include <mutex>
#include <vector>

class PatternManager {
public:
  PatternManager();
  /**
   * Adds a pattern into the end of the circular buffer of patterns.
   */
  void queuePattern(std::vector<sisyphus::ArmAngle> pattern);

  void step();

private:
  ServoMotor servo_motor;
  StepperMotor stepper_motor;
  std::vector<std::vector<sisyphus::ArmAngle>> patterns;
  std::vector<sisyphus::ArmAngle> current_pattern;
  std::mutex lock;
  int current_index;
  int current_pattern_index;
};

#endif
