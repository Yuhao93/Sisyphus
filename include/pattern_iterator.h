#ifndef PATTERN_WRAPPER_H
#define PATTERN_WRAPPER_H

#include "model.pb.h"

/**
 * Wraps a pattern protobuf. Acts as an iterator for the underlying pattern,
 * such that each call to next retrieves the next step to make in the pattern.
 * The underying implementation is the Bresenham line algorithm.
 */
class PatternIterator {
public:
  PatternIterator(const sisyphus::Pattern& pattern);
  const sisyphus::Pattern& pattern() const;
  bool has_next() const;
  const sisyphus::Step& next();

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
