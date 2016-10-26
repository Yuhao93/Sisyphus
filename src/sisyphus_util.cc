#include "sisyphus_util.h"

#include "structs.h"
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

PolarCoordinate SisyphusUtil::PolarFromCartesian(
    CartesianCoordinate& coordinate) {
  float x = coordinate.x;
  float y = coordinate.y;
  float r = sqrt(x * x + y * y);
  float a = atan2(y, x);
  PolarCoordinate new_coordinate = { r, a };
  return new_coordinate;
}

CartesianCoordinate SisyphusUtil::CartesianFromPolar(
    PolarCoordinate& coordinate) {
  float r = coordinate.r;
  float a = coordinate.a;
  float x = r * cos(a);
  float y = r * sin(a);
  CartesianCoordinate new_coordinate = { x, y };
  return new_coordinate;
}

ArmAngle SisyphusUtil::ArmAngleFromPolar(
    PolarCoordinate& coordinate) {
  float servo_angle = SisyphusUtil::ClampBetween2Pi(
      2 * asin(coordinate.r / (2 * SisyphusUtil::r)));
  float stepper_angle = SisyphusUtil::ClampBetween2Pi(
      acos(coordinate.r / (2 * SisyphusUtil::r)) + coordinate.a);
  ArmAngle armAngle = {
    stepper_angle,
    servo_angle
  };
  return armAngle;
}

ArmAngle SisyphusUtil::ArmAngleFromCartesian(
    CartesianCoordinate& coordinate) {
  PolarCoordinate polar = SisyphusUtil::PolarFromCartesian(coordinate);
  return ArmAngleFromPolar(polar);
}
