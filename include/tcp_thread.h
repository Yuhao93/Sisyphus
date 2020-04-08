#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include "application.h"
#include "rpc_server.h"
#include "thread.h"

class TcpThread : public Thread {
public:
  TcpThread(Application* app, RpcServer* rpc_server)
      : app_(app), rpc_server_(rpc_server) {}
  void Init();
  void Run();
private:
  Application* app_;
  RpcServer* rpc_server_;
};

#endif
