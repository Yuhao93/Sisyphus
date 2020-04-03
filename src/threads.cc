#include "threads.h"

#include "thread.h"
#include <pthread.h>
#include <vector>

void Threads::Start(Thread* p) {
  pthread_t thread;
  p->Attach(thread);
  threads.push_back(p);
  pthread_create(&thread, NULL, &p->RunThread, NULL);
}
