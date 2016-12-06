#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include "stepper_motors.h"
#include "model.pb.h"
#include "pattern_wrapper.h"
#include <mutex>
#include <vector>

class PatternManager {
public:
  PatternManager();
  /**
   * Adds a pattern into the end of the circular buffer of patterns.
   */
  void queue_pattern(sisyphus::Pattern pattern);

  /**
   * List patterns
   */
  std::vector<sisyphus::Pattern> list_patterns();

  void step();

private:
  StepperMotors stepper_motors;
  std::vector<PatternWrapper> patterns;
  std::mutex lock;

  int pattern_index;
};

#endif
