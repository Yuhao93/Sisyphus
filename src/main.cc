#include "pattern.h"
#include "sisyphus_util.h"
#include "structs.h"
#include "threads.h"
#include <vector>
#include <cstdio>

int main() {
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
  pattern.queuePattern(angles);

  Threads::Start(&pattern);

  while(true) {
    
  }
  return 0;
}
