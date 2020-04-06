#include "threads.h"

#include "thread.h"
#include "thread_wrapper.h"
#include <vector>

void Threads::Start(Thread* p) {
  threads.push_back(p);
  thread_wrappers.push_back(ThreadWrapper(p));
}
