#include "thread_wrapper.h"
#include "thread.h"

void* RunThread(void* context) {
  return ((Thread*) context)->RunThread(nullptr);
}

ThreadWrapper::ThreadWrapper(Thread* p) {
  pthread_create(&thread, NULL, &RunThread, p);
}
