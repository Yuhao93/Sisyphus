#ifndef INITIALIZING_PATTERN_ITERATOR_H
#define INITIALIZING_PATTERN_ITERATOR_H

#include "abstract_pattern_iterator.h"
#include "model.pb.h"

#define INTIALIZING_STAGE_ZEROING 1
#define INTIALIZING_STAGE_TARGETING 2

/**
 * Pattern iterator that:
 * 1) resets the linear position to 0.
 * 2) Moves the linear position to the starting position of the given pattern.
 */
class InitializingPatternIterator : public AbstractPatternIterator {
public:
  InitializingPatternIterator(const sisyphus::Pattern& pattern);
  bool has_next() const;
  const sisyphus::Step& next();
  bool is_external_pattern() const;

private:
  int target;
  int current;
  int stage;
  bool complete;
};

#endif
