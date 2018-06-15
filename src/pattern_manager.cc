#include "pattern_manager.h"

#include "sisyphus_util.h"
#include "stepper_motors.h"
#include "model.pb.h"
#include "pattern_iterator.h"
#include "wiring_pi_wrapper.h"
#include <cstdint>

PatternManager::PatternManager() : pattern_index(0) {
  wiringPiSetup();
  stepper_motors.setup();
}

void PatternManager::queue_pattern(const sisyphus::Pattern& pattern) {
  lock.lock();
  PatternIterator p(pattern);
  if (patterns.empty()) {
    pattern_index = 0;
  } else {
    pattern_index++;
  }
  patterns.push_back(p);
  lock.unlock();
}

const std::vector<sisyphus::Pattern> PatternManager::list_patterns() {
  std::vector<sisyphus::Pattern> patterns_copy;
  lock.lock();
  for (const auto& pattern : patterns) {
    patterns_copy.push_back(pattern.pattern());
  }
  lock.unlock();
  return patterns_copy;
}

void PatternManager::step() {
  lock.lock();
  if (patterns.size() == 0) {
    lock.unlock();
    return;
  }
  if (pattern_index >= patterns.size()) {
    pattern_index = 0;
  }
  if (!patterns[pattern_index].has_next()) {
    pattern_index++;
    lock.unlock();
    return;
  }
  lock.unlock();
  stepper_motors.step(patterns[pattern_index].next());
}
