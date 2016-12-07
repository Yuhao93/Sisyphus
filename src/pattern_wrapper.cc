#include "pattern_wrapper.h"

#include "model.pb.h"
#include "sisyphus_util.h"
#include "table_constants.h"
#include <cstdlib>

namespace {
sisyphus::Step default_step;

int xy_swapped(int octant) {
  switch (octant) {
    case 1:
    case 2:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

}

int PatternWrapper::get_octant(const sisyphus::Segment& segment) {
  int angular_change =
      SisyphusUtil::DiffBetweenAngles(
          segment.end().angular_value(),
          segment.start().angular_value());
  int linear_change =
      segment.end().linear_value() - segment.start().linear_value();
  if (linear_change >= 0 && angular_change >= 0) {
    if (linear_change <= angular_change) {
      return 0;
    } else {
      return 1;
    }
  } else if (linear_change >= 0 && angular_change < 0) {
    if (linear_change > abs(angular_change)) {
      return 2;
    } else {
      return 3;
    }
  } else if (linear_change < 0 && angular_change < 0) {
    if (abs(linear_change) <= abs(angular_change)) {
      return 4;
    } else {
      return 5;
    }
  } else if (abs(linear_change) <= angular_change) {
    return 6;
  } else {
    return 7;
  }
}

void PatternWrapper::transform_input(
      int& x, int& y, const sisyphus::Segment_Point& point) {
  switch (octant) {
    case 0:
      x = point.angular_value();
      y = point.linear_value();
      return;
    case 1:
      x = point.linear_value();
      y = point.angular_value();
      return;
    case 2:
      x = point.linear_value();
      y = -point.angular_value();
      return;
    case 3:
      x = -point.angular_value();
      y = point.linear_value();
      return;
    case 4:
      x = -point.angular_value();
      y = -point.linear_value();
      return;
    case 5:
      x = -point.linear_value();
      y = -point.angular_value();
      return;
    case 6:
      x = -point.linear_value();
      y = point.angular_value();
      return;
    case 7:
      x = point.angular_value();
      y = -point.linear_value();
      return;
  }
}

void PatternWrapper::transform_output(
    int x, int y, int& angular_value, int& linear_value) {
  switch (octant) {
    case 0:
      angular_value = x;
      linear_value = y;
      return;
    case 1:
      angular_value = y;
      linear_value = x;
      return;
    case 2:
      angular_value = -y;
      linear_value = x;
      return;
    case 3:
      angular_value = -x;
      linear_value = y;
      return;
    case 4:
      angular_value = -x;
      linear_value = -y;
      return;
    case 5:
      angular_value = -y;
      linear_value = -x;
      return;
    case 6:
      angular_value = y;
      linear_value = -x;
      return;
    case 7:
      angular_value = x;
      linear_value = -y;
      return;
  }
}

void PatternWrapper::init_segment(const sisyphus::Segment& segment) {
  octant = get_octant(segment);
  int end_x;
  int end_y;
  transform_input(x, y, segment.start());
  transform_input(end_x, end_y, segment.end());
  target_x = end_x;
  dx = xy_swapped(octant) ? end_x - x : SisyphusUtil::DiffBetweenAngles(end_x, x);
  dy = xy_swapped(octant) ? SisyphusUtil::DiffBetweenAngles(end_y, y): end_y - y;
  D = 2 * (dy) - (dx);
  prev_x = x;
  prev_y = y;
}

PatternWrapper::PatternWrapper(const sisyphus::Pattern& pattern) : p(pattern) {
  segment_index = 0;
  if (p.path_segment().empty()) {
    return;
  }
  init_segment(p.path_segment(segment_index));
}

sisyphus::Pattern PatternWrapper::pattern() {
  return p;
}

bool PatternWrapper::has_next() {
  return segment_index < p.path_segment().size();
}

sisyphus::Step PatternWrapper::next() {
  if (!has_next()) {
    return default_step;
  }
  sisyphus::Step step;
  int angular_value;
  int prev_angular_value;
  int linear_value;
  int prev_linear_value;
  transform_output(x, y, angular_value, linear_value);
  transform_output(prev_x, prev_y, prev_angular_value, prev_linear_value);
  int angle_diff =
      SisyphusUtil::DiffBetweenAngles(angular_value, prev_angular_value);
  if (angle_diff > 0) {
    step.set_angular_movement(sisyphus::Step::FORWARDS);
  } else if (angle_diff < 0) {
    step.set_angular_movement(sisyphus::Step::BACKWORDS);
  } else {
    step.set_angular_movement(sisyphus::Step::STOP);
  }
  if (linear_value > prev_linear_value) {
    step.set_linear_movement(sisyphus::Step::FORWARDS);
  } else if (linear_value < prev_linear_value) {
    step.set_linear_movement(sisyphus::Step::BACKWORDS);
  } else {
    step.set_linear_movement(sisyphus::Step::STOP);
  }
  prev_x = x;
  prev_y = y;
  if (x == target_x) {
    segment_index++;
    if (has_next()) {
      init_segment(p.path_segment(segment_index));
    }
  } else {
    x++;
    if (!xy_swapped(octant) && x == ANGULAR_STEPS_PER_REVOLUTION) {
      x = 0;
    }
    if (D > 0) {
      y++;
      if (xy_swapped(octant) && y == ANGULAR_STEPS_PER_REVOLUTION) {
        y = 0;
      }
      D -= dx;
    }
    D += dy;
  }
  return step;
}
