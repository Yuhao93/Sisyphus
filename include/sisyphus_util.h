#ifndef SISYPHUS_UTIL_H
#define SISYPHUS_UTIL_H

#include "structs.h"

namespace SisyphusUtil {
extern float pi;
extern float r;
float ClampBetween2Pi(float val);
float DiffBetweenAngles(float angle1, float angle2);
PolarCoordinate PolarFromCartesian(CartesianCoordinate& coordinate);
CartesianCoordinate CartesianFromPolar(PolarCoordinate& coordinate);
ArmAngle ArmAngleFromPolar(PolarCoordinate& coordinate);
ArmAngle ArmAngleFromCartesian(CartesianCoordinate& coordinate);
}

#endif
