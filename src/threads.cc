#include "threads.h"

#include "model.pb.h"
#include "pattern_manager.h"
#include "tcp_server.h"
#include "wiring_pi_wrapper.h"
#include <pthread.h>
#include <vector>

namespace ThreadsImpl {
  void* do_pattern(void*) {
    while(true) {
      if (Threads::pattern_manager != nullptr) {
        Threads::pattern_manager->step();
      }
    }
  }

  void* do_tcp(void*) {
    TcpServerSetup();
    while(true) {
      TcpServerProcess();
    }
  }
}

PatternManager* Threads::pattern_manager;
std::vector<pthread_t> Threads::threads;

void Threads::QueuePattern(sisyphus::Pattern pattern) {
  if (Threads::pattern_manager != nullptr) {
    Threads::pattern_manager->queue_pattern(pattern);
  }
}

void Threads::Start(PatternManager* p) {
  Threads::pattern_manager = p;
  pthread_t pattern_thread;
  //pthread_t tcp_thread;
  Threads::threads.push_back(pattern_thread);
  //Threads::threads.push_back(tcp_thread);
  pthread_create(&Threads::threads[0], NULL, &ThreadsImpl::do_pattern, NULL);
  //pthread_create(&Threads::threads[1], NULL, &ThreadsImpl::do_tcp, NULL);
}
