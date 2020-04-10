#include "sisyphus_util.h"

#include "model.pb.h"
#include "table_constants.h"
#include "parametric_processor.h"
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
          SisyphusUtil::ClampBetween2Pi(polar.a()) / (2 * SisyphusUtil::pi)));
  point->set_linear_value((int) (LINEAR_STEPS_PER_SWEEP * polar.r()));
}

void point_to_polar(
    sisyphus::PolarCoordinate* polar,
    const sisyphus::Segment_Point& point) {
  polar->set_a((float) (point.angular_value() * 2 * SisyphusUtil::pi) / (float) ANGULAR_STEPS_PER_REVOLUTION);
  polar->set_r((float) point.linear_value() / (float) LINEAR_STEPS_PER_SWEEP);
}

sisyphus::Segment segment_from_points(
    const sisyphus::PolarCoordinate& start,
    const sisyphus::PolarCoordinate& end) {
  sisyphus::Segment segment;
  polar_to_point(segment.mutable_start(), start);
  polar_to_point(segment.mutable_end(), end);
  return segment;
}

bool segment_is_point(const sisyphus::Segment& segment) {
  return segment.start().angular_value() == segment.end().angular_value()
      && segment.start().linear_value() == segment.end().linear_value();
}

template<class Coord>
std::vector<Coord> ToVector(
    ::google::protobuf::RepeatedPtrField<Coord> coords) {
  std::vector<Coord> vec;
  for (const auto& coord : coords) {
    vec.push_back(coord);
  }
  return vec;
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
      ? (ANGULAR_STEPS_PER_REVOLUTION - diff_magnitude) * (diff < 0 ? 1 : -1)
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

sisyphus::Pattern SisyphusUtil::PatternFromPolarCoordinates(
    const std::vector<sisyphus::PolarCoordinate>& coordinates) {
  sisyphus::Pattern p;
  if (coordinates.empty()) {
    return p;
  }
  if (coordinates.size() == 1) {
    *p.add_path_segment() =
        segment_from_points(coordinates[0], coordinates[0]);
    return p;
  }
  sisyphus::PolarCoordinate prev = coordinates[0];
  for (int i = 1; i < coordinates.size(); i++) {
    sisyphus::PolarCoordinate current = coordinates[i];
    sisyphus::Segment segment = segment_from_points(prev, current);
    if (!segment_is_point(segment)) {
      *p.add_path_segment() = segment;
    }
    prev = current;
  }
  if (p.path_segment().empty()) {
    *p.add_path_segment() = segment_from_points(prev, prev);
  }
  return p;
}

sisyphus::Pattern SisyphusUtil::PatternFromCartesianCoordinates(
    const std::vector<sisyphus::CartesianCoordinate>& coordinates) {
  std::vector<sisyphus::PolarCoordinate> polar;
  for (const sisyphus::CartesianCoordinate& coordinate : coordinates) {
    polar.push_back(SisyphusUtil::PolarFromCartesian(coordinate));
  }
  return SisyphusUtil::PatternFromPolarCoordinates(polar);
}

sisyphus::Pattern SisyphusUtil::StoredPatternToPattern(
    const sisyphus::StoredPattern& pattern) {
  if (pattern.has_cartesian()) {
    return SisyphusUtil::PatternFromCartesianCoordinates(ToVector(pattern.cartesian().coordinate()));
  } else if (pattern.has_polar()) {
    return SisyphusUtil::PatternFromPolarCoordinates(ToVector(pattern.polar().coordinate()));
  } else if (pattern.has_parametric()) {
    return SisyphusUtil::PatternFromCartesianCoordinates(
        ParametricProcessor::GetCoordinatesFromParametricFunction(pattern.parametric()));
  }
  return sisyphus::Pattern();
}

sisyphus::StoredPattern SisyphusUtil::PatternToPolarStoredPattern(
    const sisyphus::Pattern& pattern) {
  sisyphus::StoredPattern stored_pattern;
  if (pattern.path_segment_size() == 0) {
      return stored_pattern;
  }

  sisyphus::PolarCoordinate* coord =
      stored_pattern.mutable_polar()->add_coordinate();
  point_to_polar(coord, pattern.path_segment(0).start());
  for (const auto& segment : pattern.path_segment()) {
    coord = stored_pattern.mutable_polar()->add_coordinate();
    point_to_polar(coord, segment.end());
  }

  return stored_pattern;
}
