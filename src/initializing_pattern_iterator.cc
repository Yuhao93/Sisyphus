#include "initializing_pattern_iterator.h"

#include "gpio.h"
#include "wiring_pi_wrapper.h"

InitializingPatternIterator::InitializingPatternIterator(const sisyphus::Pattern& pattern)
    : stage(INTIALIZING_STAGE_ZEROING), current(0), complete(false) {
  if (pattern.path_segment().size() < 1) {
    return;
  }
  target = pattern.path_segment(0).start().linear_value();
}

bool InitializingPatternIterator::has_next() const {
  return complete;
}

const sisyphus::Step& InitializingPatternIterator::next() {
  switch (stage) {
    case INTIALIZING_STAGE_ZEROING:
      if (Gpio::read(GPIO_LINEAR_FEEDBACK) == LOW) {
        sisyphus::Step backward;
        backward.set_linear_movement(sisyphus::Step::BACKWARDS);
        return std::move(backward);
      }
      // continue immediately into next stage
      stage = INTIALIZING_STAGE_TARGETING;
    case INTIALIZING_STAGE_TARGETING:
      if (current >= target) {
        complete = true;
        sisyphus::Step stop;
        return std::move(stop);
      }
      current++;
      sisyphus::Step forward;
      forward.set_linear_movement(sisyphus::Step::FORWARDS);
      return std::move(forward);
  }
}

bool InitializingPatternIterator::is_external_pattern() const {
  return false;
}
