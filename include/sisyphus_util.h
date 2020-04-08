#ifndef SISYPHUS_UTIL_H
#define SISYPHUS_UTIL_H

#include "model.pb.h"
#include <vector>

namespace SisyphusUtil {
extern float pi;
extern float r;
float ClampBetween2Pi(float val);
int DiffBetweenAngles(int angle1, int angle2);
sisyphus::PolarCoordinate PolarFromCartesian(
    const sisyphus::CartesianCoordinate& coordinate);
sisyphus::Pattern PatternFromPolarCoordinates(
    const std::vector<sisyphus::PolarCoordinate>& coords);
sisyphus::Pattern PatternFromCartesianCoordinates(
    const std::vector<sisyphus::CartesianCoordinate>& coords);
sisyphus::Pattern StoredPatternToPattern(const sisyphus::StoredPattern& pattern);
sisyphus::StoredPattern PatternToPolarStoredPattern(const sisyphus::Pattern& pattern);
}

#endif
