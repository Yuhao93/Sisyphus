#include "pattern_manager.h"

#include "sisyphus_util.h"
#include "stepper_motors.h"
#include "model.pb.h"
#include "pattern_wrapper.h"
#include "wiring_pi_wrapper.h"
#include <cstdint>

PatternManager::PatternManager() : pattern_index(0) {
  wiringPiSetup();
  stepper_motors.setup();
}

void PatternManager::queue_pattern(sisyphus::Pattern pattern) {
  lock.lock();
  PatternWrapper p(pattern);
  if (patterns.empty()) {
    patterns.push_back(p);
    pattern_index = 0;
  } else {
    patterns.push_back(p);
    pattern_index++;
  }
  lock.unlock();
}

std::vector<sisyphus::Pattern> PatternManager::list_patterns() {
  std::vector<sisyphus::Pattern> patterns_copy;
  lock.lock();
  for (auto it = patterns.begin(); it != patterns.end(); it++) {
    patterns_copy.push_back(it->pattern());
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
  PatternWrapper& pattern = patterns[pattern_index];
  if (!pattern.has_next()) {
    pattern_index++;
    lock.unlock();
    return;
  }
  lock.unlock();
  stepper_motors.step(pattern.next());
  delay(2);
}
