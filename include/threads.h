#ifndef THREADS_H
#define THREADS_H

#include "pattern_manager.h"
#include "model.pb.h"
#include <pthread.h>
#include <vector>

namespace Threads {
  void QueuePattern(sisyphus::Pattern pattern);
  void Start(PatternManager* pattern_manager);

  extern PatternManager* pattern_manager;
  extern std::vector<pthread_t> threads;
}

#endif
