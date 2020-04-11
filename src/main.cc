#include "application.h"
#include "sisyphus_util.h"
#include "model.pb.h"
#include "thread.h"
#include "threads.h"
#include "pattern_thread.h"
#include "tcp_thread.h"
#include <vector>
#include <cstdio>

int main() {
  Application app;
  Threads threads;
  RpcServer rpc_server;
  Thread* pattern_thread = new PatternThread(&app);
  Thread* tcp_thread = new TcpThread(&app, &rpc_server);

  threads.Start(pattern_thread);
  threads.Start(tcp_thread);

  while(true) { }
  return 0;
}
