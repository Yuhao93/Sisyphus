#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include "stepper_motors.h"
#include "model.pb.h"
#include "pattern_iterator.h"
#include <mutex>
#include <vector>

class PatternManager {
public:
  PatternManager();
  /**
   * Adds a pattern into the end of the circular buffer of patterns.
   */
  void queue_pattern(const sisyphus::Pattern& pattern);

  /**
   * List patterns
   */
  const std::vector<sisyphus::Pattern> list_patterns();

  void step();

private:
  StepperMotors stepper_motors;
  std::vector<PatternIterator> patterns;
  std::mutex lock;

  int pattern_index;
};

#endif
