#ifndef ABSTRACT_PATTERN_ITERATOR_H
#define ABSTRACT_PATTERN_ITERATOR_H

#include "model.pb.h"

/**
 * Abstract PatterIterator that the pattern namager can iterate over to get
 * stepper motor instructions
 */
class AbstractPatternIterator {
 public:
  virtual bool has_next() const = 0;
  virtual sisyphus::Step next() = 0;
  virtual bool is_external_pattern() const = 0;
};

#endif
