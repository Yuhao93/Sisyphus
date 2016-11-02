#include "pattern_manager.h"
#include "sisyphus_util.h"
#include "model.pb.h"
#include "threads.h"
#include <vector>
#include <cstdio>

int main() {
  PatternManager patternManager;

  std::vector<sisyphus::CartesianCoordinate> square;
  float side_length = 1.4f;
  int sample_per_side = 100;
  for (int i = 0; i < sample_per_side; i++) {
    sisyphus::CartesianCoordinate coordinate;
    coordinate.set_x(-side_length / 2 + (i * side_length / sample_per_side));
    coordinate.set_y(-side_length / 2);
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    sisyphus::CartesianCoordinate coordinate;
    coordinate.set_x(side_length / 2);
    coordinate.set_y(-side_length / 2 + (i * side_length / sample_per_side));
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    sisyphus::CartesianCoordinate coordinate;
    coordinate.set_x(side_length / 2 - (i * side_length / sample_per_side));
    coordinate.set_y(side_length / 2);
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    sisyphus::CartesianCoordinate coordinate;
    coordinate.set_x(-side_length / 2);
    coordinate.set_y(side_length / 2 - (i * side_length / sample_per_side));
    square.push_back(coordinate);
  }
  std::vector<sisyphus::ArmAngle> angles;
  for (auto it = square.begin(); it != square.end(); it++) {
    angles.push_back(SisyphusUtil::ArmAngleFromCartesian(*it));
  }
  patternManager.queuePattern(angles);

  Threads::Start(&patternManager);

  while(true) {

  }
  return 0;
}
