#ifndef SISYPHUS_UTIL_H
#define SISYPHUS_UTIL_H

#include "model.pb.h"

namespace SisyphusUtil {
extern float pi;
extern float r;
float ClampBetween2Pi(float val);
int DiffBetweenAngles(int angle1, int angle2);
sisyphus::PolarCoordinate PolarFromCartesian(
    const sisyphus::CartesianCoordinate& coordinate);
sisyphus::Segment SegmentFromPolarCoordinates(
    const sisyphus::PolarCoordinate& start,
    const sisyphus::PolarCoordinate& end);
sisyphus::Segment SegmentFromCartesianCoordinates(
    const sisyphus::CartesianCoordinate& start,
    const sisyphus::CartesianCoordinate& end);
}

#endif
