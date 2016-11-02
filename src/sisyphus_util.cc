#include "sisyphus_util.h"

#include "model.pb.h"
#include <vector>
#include <cmath>
#include <cstdio>

float SisyphusUtil::pi = 3.14159f;
float SisyphusUtil::r = .5f;

float SisyphusUtil::ClampBetween2Pi(float val) {
  bool negative = val < 0;
  float absVal = negative ? val * -1 : val;
  int overflow = (int) floor(absVal / SisyphusUtil::pi);
  if (overflow % 2 == 1) {
    overflow++;
  }
  return val - ((negative ? -1 : 1) * overflow * SisyphusUtil::pi);
}

float SisyphusUtil::DiffBetweenAngles(float angle1, float angle2) {
  float diff = angle1 - angle2;
  if (fabs(diff) > SisyphusUtil::pi) {
    if (diff < 0) {
      return (2 * SisyphusUtil::pi) + diff;
    } else if (diff > 0) {
      return diff - (2 * SisyphusUtil::pi);
    }
  }
  return diff;
}

sisyphus::PolarCoordinate SisyphusUtil::PolarFromCartesian(
    sisyphus::CartesianCoordinate& coordinate) {
  float x = coordinate.x();
  float y = coordinate.y();
  float r = sqrt(x * x + y * y);
  float a = atan2(y, x);
  sisyphus::PolarCoordinate new_coordinate;
  new_coordinate.set_r(r);
  new_coordinate.set_a(a);
  return new_coordinate;
}

sisyphus::CartesianCoordinate SisyphusUtil::CartesianFromPolar(
    sisyphus::PolarCoordinate& coordinate) {
  float r = coordinate.r();
  float a = coordinate.a();
  float x = r * cos(a);
  float y = r * sin(a);
  sisyphus::CartesianCoordinate new_coordinate;
  new_coordinate.set_x(x);
  new_coordinate.set_y(y);
  return new_coordinate;
}

sisyphus::ArmAngle SisyphusUtil::ArmAngleFromPolar(
    sisyphus::PolarCoordinate& coordinate) {
  float servo_angle = SisyphusUtil::ClampBetween2Pi(
      2 * asin(coordinate.r() / (2 * SisyphusUtil::r)));
  float stepper_angle = SisyphusUtil::ClampBetween2Pi(
      acos(coordinate.r() / (2 * SisyphusUtil::r)) + coordinate.a());
  sisyphus::ArmAngle arm_angle;
  arm_angle.set_stepper_angle(stepper_angle);
  arm_angle.set_servo_angle(servo_angle);
  return arm_angle;
}

sisyphus::ArmAngle SisyphusUtil::ArmAngleFromCartesian(
    sisyphus::CartesianCoordinate& coordinate) {
  sisyphus::PolarCoordinate polar =
      SisyphusUtil::PolarFromCartesian(coordinate);
  return ArmAngleFromPolar(polar);
}
