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
  void queuePattern(sisyphus::Pattern pattern);

  /**
   * List patterns
   */
  std::vector<sisyphus::Pattern> listPatterns();

  void step();

private:
  ServoMotor servo_motor;
  StepperMotor stepper_motor;
  std::vector<sisyphus::Pattern> patterns;
  sisyphus::Pattern current_pattern;
  std::mutex lock;
  int current_index;
  int current_pattern_index;
};

#endif
