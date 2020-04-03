#ifndef APPLICATION_H
#define APPLICATION_H

class Application {
public:
  Application() {}
  
  PatternManager pattern_manager;
  Led led;
  RpcServer rpc_server;
  Socket socket;
}

#endif
