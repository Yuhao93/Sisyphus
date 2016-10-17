#include "pattern.h"
#include "sisyphus_util.h"
#include "structs.h"
#include <vector>
#include <cstdio>

int main() {
  printf("Initializing Pattern\n");
  Pattern pattern;

  std::vector<CartesianCoordinate> square;
  float side_length = 1.4f;
  int sample_per_side = 100;
  for (int i = 0; i < sample_per_side; i++) {
    CartesianCoordinate coordinate = {
      -side_length / 2 + (i * side_length / sample_per_side),
      -side_length / 2
    };
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    CartesianCoordinate coordinate = {
      side_length / 2,
      -side_length / 2 + (i * side_length / sample_per_side)
    };
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    CartesianCoordinate coordinate = {
      side_length / 2 - (i * side_length / sample_per_side),
      side_length / 2
    };
    square.push_back(coordinate);
  }
  for (int i = 0; i < sample_per_side; i++) {
    CartesianCoordinate coordinate = {
      -side_length / 2,
      side_length / 2 - (i * side_length / sample_per_side)
    };
    square.push_back(coordinate);
  }
  std::vector<ArmAngle> angles;
  for (auto it = square.begin(); it != square.end(); it++) {
    angles.push_back(SisyphusUtil::ArmAngleFromCartesian(*it));
  }
  printf("Finished Initializing Pattern\n");
  pattern.queuePattern(angles);
  printf("Running...\n");
  while(true) {
    pattern.step();
  }
  return 0;
}
