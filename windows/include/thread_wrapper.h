#ifndef THREAD_WRAPPER_H
#define THREAD_WRAPPER_H

#include "thread.h"

#include <windows.h>

class ThreadWrapper {
public:
  ThreadWrapper(Thread* p);
private:
  HANDLE handle;
};

#endif
