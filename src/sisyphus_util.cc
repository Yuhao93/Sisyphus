#include "sisyphus_util.h"

#include "model.pb.h"
#include "table_constants.h"
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void polar_to_point(
    sisyphus::Segment_Point* point,
    const sisyphus::PolarCoordinate& polar) {
  point->set_angular_value(
      (int) (ANGULAR_STEPS_PER_REVOLUTION *
          SisyphusUtil::ClampBetween2Pi(polar.a())));
  point->set_linear_value((int) (LINEAR_STEPS_PER_SWEEP * polar.r()));
}
}

float SisyphusUtil::pi = 3.14159f;
float SisyphusUtil::r = .5f;

float SisyphusUtil::ClampBetween2Pi(float val) {
  float pi_2 = 2 * SisyphusUtil::pi;
  if (val < 0) {
    int cycles = (int) (-val / pi_2);
    return val + (cycles + 1) * pi_2;
  } else if (val >= pi_2) {
    int cycles = (int) (val / pi_2);
    return val - cycles * pi_2;
  }
  return val;
}

int SisyphusUtil::DiffBetweenAngles(int angle1, int angle2) {
  int diff = angle1 - angle2;
  int diff_magnitude = abs(diff);
  return diff_magnitude > (ANGULAR_STEPS_PER_REVOLUTION / 2)
      ? (ANGULAR_STEPS_PER_REVOLUTION - diff_magnitude) * (diff < 0 ? -1 : 1)
      : diff;
}

sisyphus::PolarCoordinate SisyphusUtil::PolarFromCartesian(
    const sisyphus::CartesianCoordinate& coordinate) {
  float x = coordinate.x();
  float y = coordinate.y();
  float r = sqrt(x * x + y * y);
  float a = atan2(y, x);
  sisyphus::PolarCoordinate new_coordinate;
  new_coordinate.set_r(r);
  new_coordinate.set_a(a);
  return new_coordinate;
}

sisyphus::Segment SisyphusUtil::SegmentFromPolarCoordinates(
    const sisyphus::PolarCoordinate& start,
    const sisyphus::PolarCoordinate& end) {
  sisyphus::Segment segment;
  sisyphus::Segment_Point* start_point = segment.mutable_start();
  sisyphus::Segment_Point* end_point = segment.mutable_end();
  polar_to_point(start_point, start);
  polar_to_point(end_point, end);
  return segment;
}

sisyphus::Segment SisyphusUtil::SegmentFromCartesianCoordinates(
    const sisyphus::CartesianCoordinate& start,
    const sisyphus::CartesianCoordinate& end) {
  return SisyphusUtil::SegmentFromPolarCoordinates(
      SisyphusUtil::PolarFromCartesian(start),
      SisyphusUtil::PolarFromCartesian(end));
}
