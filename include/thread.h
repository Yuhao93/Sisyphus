#ifndef THREAD_H
#define THREAD_H

#include <cstdio>

class Thread {
public:
  void* RunThread(void* input) {
    Init();
    while(true) {
      Run();
    }
  }

  virtual void Init() {}
  virtual void Run() = 0;
};

#endif
