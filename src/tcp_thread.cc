#include "tcp_thread.h"

void TcpThread::Init() {
  socket_->Init();
}

void TcpThread::Run() {
  socket_->Write(rpc_server->HandleMessage(socket_->Read(), app_));
}
