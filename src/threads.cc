#include "threads.h"

#include "thread.h"
#include <pthread.h>
#include <vector>

void* RunThread(void* context) {
  return ((Thread*) context)->RunThread(nullptr);  
}

void Threads::Start(Thread* p) {
  pthread_t thread;
  p->Attach(thread);
  threads.push_back(p);
  pthread_create(&thread, NULL, &RunThread, p);
}
