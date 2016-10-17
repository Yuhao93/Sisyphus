#include "sisyphus_util.h"

#include "structs.h"
#include <vector>
#include <cmath>

float SisyphusUtil::pi = 3.14159f;
float SisyphusUtil::r = .5f;

float SisyphusUtil::ClampBetween2Pi(float val) {
  float overflow = floor(val / SisyphusUtil::pi);
  return val - (overflow * 2 * SisyphusUtil::pi);
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
  float servo_angle = 2 * asin(coordinate.r / (2 * SisyphusUtil::r));
  float stepper_angle =
    acos(coordinate.r / (2 * SisyphusUtil::r)) + coordinate.a;
  ArmAngle armAngle = {
    SisyphusUtil::ClampBetween2Pi(stepper_angle),
    SisyphusUtil::ClampBetween2Pi(SisyphusUtil::pi + servo_angle)
  };
  return armAngle;
}

ArmAngle SisyphusUtil::ArmAngleFromCartesian(
    CartesianCoordinate& coordinate) {
  PolarCoordinate polar = SisyphusUtil::PolarFromCartesian(coordinate);
  return ArmAngleFromPolar(polar);
}
