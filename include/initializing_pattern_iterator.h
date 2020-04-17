#ifndef INITIALIZING_PATTERN_ITERATOR_H
#define INITIALIZING_PATTERN_ITERATOR_H

#include "abstract_pattern_iterator.h"
#include "enqueued_pattern_iterator.h"
#include "model.pb.h"

#define INITIALIZING_STAGE_ZEROING 1
#define INITIALIZING_STAGE_CENTERING 2
#define INITIALIZING_STAGE_CLEARING 3
#define INITIALIZING_STAGE_TARGETING 4

#define ZEROING_THRESHOLD 100

/**
 * Pattern iterator that:
 * 1) resets the linear position to 0
 * 2) moves the linear position to be at the center
 * 3) sweeps through the entire visible range to clear all marks
 * 4) backtracks to where the target is
 */
class InitializingPatternIterator : public AbstractPatternIterator {
public:
  InitializingPatternIterator(const sisyphus::Pattern& pattern);
  bool has_next() const;
  sisyphus::Step next();
  bool is_external_pattern() const;

private:
  int target;
  int current;
  int stage;
  int zero_count;
  int center_steps;
  bool complete;
  EnqueuedPatternIterator clearing_pattern;
  EnqueuedPatternIterator targeting_pattern;
};

#endif
