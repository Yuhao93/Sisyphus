#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include "application.h"
#include "thread.h"

class TcpThread : public Thread {
public:
  TcpThread(Application* app) : app_(app) {}
  void Init();
  void Run();
private:
  Application* app_;
};

#endif
