#ifndef THREADS_H
#define THREADS_H

#include "pattern_manager.h"
#include "model.pb.h"
#include <thread>
#include <vector>

namespace Threads {
  void QueuePattern(sisyphus::Pattern pattern);
  void Start(PatternManager* pattern_manager);

  extern PatternManager* pattern_manager;
  extern std::vector<std::thread> threads;
}

#endif
