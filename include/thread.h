#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread {
public:
  void Attach(pthread_t thread) {
    pthread = thread;
  }
  void* RunThread(void* input) {
    Init();
    while(true) {
      Run();
    }
  }

  virtual void Init() {}
  virtual void Run() = 0;
  
  pthread_t pthread;
};

#endif
