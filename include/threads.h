#ifndef THREADS_H
#define THREADS_H

#include "thread.h"
#include "thread_wrapper.h"
#include <vector>

class Threads {
public:
  void Start(Thread* thread);
private:
  std::vector<Thread*> threads;
  std::vector<ThreadWrapper> thread_wrappers;
};

#endif
