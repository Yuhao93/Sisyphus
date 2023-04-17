#ifndef ENQUEUED_PATTERN_ITERATOR_H
#define ENQUEUED_PATTERN_ITERATOR_H

#include "abstract_pattern_iterator.h"
#include "model.pb.h"

/**
 * Wraps a pattern protobuf. Acts as an iterator for the underlying pattern,
 * such that each call to next retrieves the next step to make in the pattern.
 * The underying implementation is the Bresenham line algorithm.
 */
class EnqueuedPatternIterator : public AbstractPatternIterator {
public:
  EnqueuedPatternIterator(const sisyphus::Pattern& pattern);
  const sisyphus::Pattern& pattern() const;
  bool has_next() const;
  sisyphus::Step next();
  bool is_external_pattern() const;
  void reset();

private:
  void initializeSegment(const sisyphus::Segment& segment);

  sisyphus::Pattern p;
  sisyphus::Segment current_segment;
  int segment_index;

  int octant;
  int x;
  int y;
  int dx;
  int dy;
  int D;

  bool reverseX;
  bool reverseY;
};

#endif
