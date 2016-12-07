#ifndef PATTERN_WRAPPER_H
#define PATTERN_WRAPPER_H

#include "model.pb.h"

/**
 * Wraps a pattern protobuf. Acts as an iterator for the underlying pattern,
 * such that each call to next retrieves the next step to make in the pattern.
 * The underying implementation is the Bresenham line algorithm.
 */
class PatternWrapper {
public:
  PatternWrapper(const sisyphus::Pattern& pattern);
  sisyphus::Pattern pattern();
  bool has_next();
  sisyphus::Step next();

private:
  int get_octant(const sisyphus::Segment& segment);
  void transform_input(int& x, int& y, const sisyphus::Segment_Point& point);
  void transform_output(int x, int y, int& angular_value, int& linear_value);
  void init_segment(const sisyphus::Segment& segment);

  sisyphus::Pattern p;
  int prev_x;
  int prev_y;
  int dx;
  int dy;
  int D;
  int x;
  int y;
  int target_x;
  int octant;
  int segment_index;
};

#endif
