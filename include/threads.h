#ifndef THREADS_H
#define THREADS_H

#include "pattern.h"
#include "structs.h"
#include <thread>
#include <vector>

namespace Threads {
  void QueuePattern(std::vector<ArmAngle>& pattern);
  void Start(Pattern* pattern);

  extern Pattern* pattern;
  extern std::vector<std::thread> threads;
}

#endif
