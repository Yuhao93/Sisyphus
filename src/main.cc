#include "pattern_manager.h"
#include "sisyphus_util.h"
#include "model.pb.h"
#include "threads.h"
#include <vector>
#include <cstdio>

int main() {
  PatternManager patternManager;
  std::vector<sisyphus::CartesianCoordinate> coords;
  for (int i = 1; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(0);
    coord.set_y(-.49f * i / 25.0f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f * i / 25.0f);
    coord.set_y(-.49f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f);
    coord.set_y(-.49f + .49f * i / 25.0f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f - .49f * i / 25.0f);
    coord.set_y(0);
    coords.push_back(coord);
  }
  sisyphus::Pattern pattern = SisyphusUtil::PatternFromCartesianCoordinates(coords);
  patternManager.queue_pattern(pattern);
  Threads::Start(&patternManager);

  while(true) {
  }
  return 0;
}
