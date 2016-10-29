#include "threads.h"

#include "pattern.h"
#include <thread>
#include <vector>

namespace ThreadsImpl {
  void do_pattern() {
    while(true) {
      if (Threads::pattern != nullptr) {
        Threads::pattern->step();
      }
    }
  }

  void do_tcp() {
    while(true) {

    }
  }
}

Pattern* Threads::pattern;
std::vector<std::thread> Threads::threads;

void Threads::QueuePattern(std::vector<ArmAngle>& pattern) {
  if (Threads::pattern != nullptr) {
    Threads::pattern->queuePattern(pattern);
  }
}

void Threads::Start(Pattern* p) {
  Threads::pattern = p;
  Threads::threads.push_back(std::thread(ThreadsImpl::do_pattern));
  Threads::threads.push_back(std::thread(ThreadsImpl::do_tcp));
}
