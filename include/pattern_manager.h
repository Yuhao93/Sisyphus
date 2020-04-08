#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include "stepper_motors.h"
#include "model.pb.h"
#include "abstract_pattern_iterator.h"
#include "enqueued_pattern_iterator.h"
#include <mutex>
#include <deque>

class PatternManager {
public:
  PatternManager();
  /**
   * Adds a pattern into the end of the circular buffer of patterns.
   */
  void QueuePattern(const sisyphus::Pattern& pattern);

  /**
   * List patterns
   */
  const std::vector<sisyphus::Pattern> ListPatterns();

  void step();

private:
  StepperMotors stepper_motors;
  std::deque<AbstractPatternIterator*> patterns;
  std::deque<EnqueuedPatternIterator> seen_patterns;
  std::mutex lock;
};

#endif
