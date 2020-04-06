#ifndef THREADS_H
#define THREADS_H

#include "thread.h"
#include <pthread.h>
#include <vector>

class Threads {
public:
  void Start(Thread* thread);
private:
  std::vector<Thread*> threads;
};

#endif
