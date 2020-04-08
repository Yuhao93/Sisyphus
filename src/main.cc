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
/*
  std::vector<sisyphus::CartesianCoordinate> coords;
  for (int i = 1; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(0);
    coord.set_y(-.49f * i / 25.0f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f * i / 25.0f);
    coord.set_y(-.49f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f);
    coord.set_y(-.49f + .49f * i / 25.0f);
    coords.push_back(coord);
  }
  for (int i = 0; i < 25; i++) {
    sisyphus::CartesianCoordinate coord;
    coord.set_x(.49f - .49f * i / 25.0f);
    coord.set_y(0);
    coords.push_back(coord);
  }

  sisyphus::Pattern pattern = SisyphusUtil::PatternFromCartesianCoordinates(coords);
  app.pattern_manager.QueuePattern(pattern);*/
  threads.Start(pattern_thread);
  threads.Start(tcp_thread);

  while(true) {
  }
  return 0;
}
