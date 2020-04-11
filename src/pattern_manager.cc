#include "pattern_manager.h"

#include "gpio.h"
#include "sisyphus_util.h"
#include "stepper_motors.h"
#include "model.pb.h"
#include "abstract_pattern_iterator.h"
#include "enqueued_pattern_iterator.h"
#include "initializing_pattern_iterator.h"
#include "wiring_pi_wrapper.h"
#include <cstdint>
#include <cstdio>
#include <mutex>

#define MAX_HISTORY 10

PatternManager::PatternManager() {
  wiringPiSetup();
  Gpio::setup();
}

void PatternManager::QueuePattern(const sisyphus::Pattern& pattern) {
  std::lock_guard<std::mutex> scoped_lock(lock);
  patterns.push_back(new InitializingPatternIterator(pattern));
  patterns.push_back(new EnqueuedPatternIterator(pattern));
}

const std::vector<sisyphus::Pattern> PatternManager::ListPatterns() {
  std::lock_guard<std::mutex> scoped_lock(lock);
  std::vector<sisyphus::Pattern> patterns_copy;
  for (const auto* pattern : patterns) {
    if (pattern->is_external_pattern()) {
      const EnqueuedPatternIterator* enqueued_pattern =
          dynamic_cast<const EnqueuedPatternIterator*>(pattern);
      patterns_copy.push_back(enqueued_pattern->pattern());
    }
  }
  return patterns_copy;
}

void PatternManager::step() {
  std::lock_guard<std::mutex> scoped_lock(lock);
  if (patterns.size() == 0 || patterns.size() > 0) {
    return;
  }
  if (!patterns.front()->has_next()) {
    AbstractPatternIterator* pattern = patterns.front();
    patterns.pop_front();
    if (pattern->is_external_pattern()) {
      EnqueuedPatternIterator* enqueued_pattern =
          dynamic_cast<EnqueuedPatternIterator*>(pattern);
      seen_patterns.push_back(*enqueued_pattern);
      if (seen_patterns.size() > MAX_HISTORY) {
        seen_patterns.pop_front();
      }
    }
    delete pattern;
  }
  if (patterns.size() > 0 ) {
    stepper_motors.step(patterns.front()->next());
  }
}
