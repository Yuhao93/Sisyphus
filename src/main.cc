#include "pattern_manager.h"
#include "sisyphus_util.h"
#include "model.pb.h"
#include "threads.h"
#include <vector>
#include <cstdio>

int main() {
  PatternManager patternManager;
  sisyphus::Pattern pattern;
  for (int i = 0; i < 1000; i++) {
    sisyphus::PolarCoordinate start;
    sisyphus::PolarCoordinate end;
    start.set_a(2 * SisyphusUtil::pi * i / 500.0f);
    start.set_r(.5f);
    end.set_a(2 * SisyphusUtil::pi * (i + 1) / 500.0f);
    end.set_r(.5f);
    sisyphus::Segment segment =
        SisyphusUtil::SegmentFromPolarCoordinates(start, end);
    pattern.add_path_segment()->CopyFrom(segment);
  }
  patternManager.queue_pattern(pattern);

  Threads::Start(&patternManager);

  while(true) {

  }
  return 0;
}
