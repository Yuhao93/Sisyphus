#ifndef PARAMETRIC_PROCESSOR_H
#define PARAMETRIC_PROCESSOR_H

#include "model.pb.h"
#include <vector>

namespace ParametricProcessor {

std::vector<sisyphus::CartesianCoordinate>
GetCoordinatesFromParametricFunction(const sisyphus::ParametricPattern& pattern);

}

#endif
