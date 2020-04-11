#include "enqueued_pattern_iterator.h"

#include "model.pb.h"
#include "sisyphus_util.h"
#include "table_constants.h"
#include <cstdlib>
#include <cmath>

namespace {
sisyphus::Step default_step;

int octantFromSegment(const sisyphus::Segment& segment) {
  int x = SisyphusUtil::DiffBetweenAngles(segment.end().angular_value(), segment.start().angular_value());
  int y = segment.end().linear_value() - segment.start().linear_value();
  double angle = 4 * atan2(y, x) / SisyphusUtil::pi;

  if (angle < 0) {
    angle += 8;
  }

  return (int) angle;
}

bool isFlipped(int octant) {
  switch(octant) {
    case 1:
    case 2:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

void setXYWithOctant(int octant, const sisyphus::Segment& segment, int& x, int& y, int& dx, int& dy, bool& reverseX, bool& reverseY) {
  int a = segment.start().angular_value();
  int l = segment.start().linear_value();
  int da = SisyphusUtil::DiffBetweenAngles(segment.end().angular_value(), a);
  int dl = segment.end().linear_value() - l;
  switch(octant) {
    case 0:
      x = a;
      dx = da;
      y = l;
      dy = dl;
      reverseX = false;
      reverseY = false;
      break;
    case 1:
      x = l;
      dx = dl;
      y = a;
      dy = da;
      reverseX = false;
      reverseY = false;
      break;
    case 2:
      x = l;
      dx = dl;
      y = -a;
      dy = -da;
      reverseX = false;
      reverseY = true;
      break;
    case 3:
      x = -a;
      dx = -da;
      y = l;
      dy = dl;
      reverseX = true;
      reverseY = false;
      break;
    case 4:
      x = -a;
      dx = -da;
      y = -l;
      dy = -dl;
      reverseX = true;
      reverseY = true;
      break;
    case 5:
      x = -l;
      dx = -dl;
      y = -a;
      dy = -da;
      reverseX = true;
      reverseY = true;
      break;
    case 6:
      x = -l;
      dx = -dl;
      y = a;
      dy = da;
      reverseX = true;
      reverseY = false;
      break;
    case 7:
      x = a;
      dx = da;
      y = -l;
      dy = -dl;
      reverseX = false;
      reverseY = true;
      break;
  }
}

void setALWithOctant(int octant, int& a, int& l, int x, int y) {
  switch(octant) {
    case 0:
      a = x;
      l = y;
      break;
    case 1:
      a = y;
      l = x;
      break;
    case 2:
      a = -y;
      l = x;
      break;
    case 3:
      a = -x;
      l = y;
      break;
    case 4:
      a = -x;
      l = -y;
      break;
    case 5:
      a = -y;
      l = -x;
      break;
    case 6:
      a = y;
      l = -x;
      break;
    case 7:
      a = x;
      l = -y;
      break;
  }
}

}

EnqueuedPatternIterator::EnqueuedPatternIterator(const sisyphus::Pattern& pattern) : p(pattern) {
  reset();
}

void EnqueuedPatternIterator::reset() {
  if (p.path_segment().size() < 1) {
    return;
  }
  segment_index = 0;
  current_segment = p.path_segment(0);
  initializeSegment(current_segment);
}

void EnqueuedPatternIterator::initializeSegment(const sisyphus::Segment& segment) {
  octant = octantFromSegment(current_segment);
  setXYWithOctant(octant, segment, x, y, dx, dy, reverseX, reverseY);
  D = 2 * dy - dx;
}

const sisyphus::Pattern& EnqueuedPatternIterator::pattern() const {
  return p;
}

bool EnqueuedPatternIterator::has_next() const {
  return segment_index < p.path_segment().size();
}

bool EnqueuedPatternIterator::is_external_pattern() const {
  return true;
}

const sisyphus::Step& EnqueuedPatternIterator::next() {
  bool is_flipped = isFlipped(octant);
  sisyphus::Step::Movement a_dir = sisyphus::Step::STOP;
  sisyphus::Step::Movement l_dir = sisyphus::Step::STOP;

  sisyphus::Step::Movement* x_dir = is_flipped ? &l_dir : &a_dir;
  sisyphus::Step::Movement* y_dir = is_flipped ? &a_dir : &l_dir;

  x++;
  *x_dir = reverseX ? sisyphus::Step::BACKWARDS : sisyphus::Step::FORWARDS;
  if (D > 0) {
    y++;
    *y_dir = reverseY ? sisyphus::Step::BACKWARDS : sisyphus::Step::FORWARDS;
    D -= 2 * dx;
  }
  D += 2 * dy;

  int* angular_value = is_flipped ? &y : &x;
  int a_value = *angular_value;
  if (a_value >= ANGULAR_STEPS_PER_REVOLUTION) {
    *angular_value -= (ANGULAR_STEPS_PER_REVOLUTION * (a_value / ANGULAR_STEPS_PER_REVOLUTION));
  }
  int a;
  int l;
  setALWithOctant(octant, a, l, x, y);
  if (a < 0) {
    a += (ANGULAR_STEPS_PER_REVOLUTION * ((-a / ANGULAR_STEPS_PER_REVOLUTION) + 1));
  }

  int target_a = current_segment.end().angular_value();
  int target_l = current_segment.end().linear_value();

  sisyphus::Step step;
  step.set_angular_movement(a_dir);
  step.set_linear_movement(l_dir);

  if (a == target_a && l == target_l) {
    if (++segment_index < p.path_segment().size()) {
      current_segment = p.path_segment(segment_index);
      initializeSegment(current_segment);
    }
  }

  return std::move(step);
}
