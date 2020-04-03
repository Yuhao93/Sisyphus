#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include "thread.h"
#include "rpc_server.h"

class TcpThread : public Thread {
public:
  TcpThread(Application* app) : app_(app), rpc_server_(app->rpc_server), socket_(app->socket) {}
  void Init();
  void Run();
private:
  Application* app_;
  RpcServer* rpc_server_;
  Socket* socket_;
};

#endif
