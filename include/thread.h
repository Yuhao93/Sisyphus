#ifndef THREAD_H
#define THREAD_H

#include <pthread>

class Thread {
public:
  void Attach(pthread thread) {
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
