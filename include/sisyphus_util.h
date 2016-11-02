#ifndef SISYPHUS_UTIL_H
#define SISYPHUS_UTIL_H

#include "model.pb.h"

namespace SisyphusUtil {
extern float pi;
extern float r;
float ClampBetween2Pi(float val);
float DiffBetweenAngles(float angle1, float angle2);
sisyphus::PolarCoordinate PolarFromCartesian(
    sisyphus::CartesianCoordinate& coordinate);
sisyphus::CartesianCoordinate CartesianFromPolar(
    sisyphus::PolarCoordinate& coordinate);
sisyphus::ArmAngle ArmAngleFromPolar(
    sisyphus::PolarCoordinate& coordinate);
sisyphus::ArmAngle ArmAngleFromCartesian(
    sisyphus::CartesianCoordinate& coordinate);
}

#endif
