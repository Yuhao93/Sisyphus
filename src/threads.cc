#include "threads.h"

#include "model.pb.h"
#include "pattern_manager.h"
#include "tcp_server.h"
#include <thread>
#include <vector>

namespace ThreadsImpl {
  void do_pattern() {
    while(true) {
      if (Threads::pattern_manager != nullptr) {
        Threads::pattern_manager->step();
      }
    }
  }

  void do_tcp() {
    TcpServerSetup();
    while(true) {
      TcpServerProcess();
    }
  }
}

PatternManager* Threads::pattern_manager;
std::vector<std::thread> Threads::threads;

void Threads::QueuePattern(sisyphus::Pattern pattern) {
  if (Threads::pattern_manager != nullptr) {
    Threads::pattern_manager->queuePattern(pattern);
  }
}

void Threads::Start(PatternManager* p) {
  Threads::pattern_manager = p;
  Threads::threads.push_back(std::thread(ThreadsImpl::do_pattern));
  Threads::threads.push_back(std::thread(ThreadsImpl::do_tcp));
}
