#ifndef THREAD_WRAPPER_H
#define THREAD_WRAPPER_H

#include "thread.h"
#include <pthread.h>

class ThreadWrapper {
public:
  ThreadWrapper(Thread* p);
private:
  pthread_t thread;
};

#endif
