#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "tcp.pb.h"

// OS specific implementations
void TcpServerSetup();
sisyphus::Request TcpNextRequest();
void TcpWriteResponse(sisyphus::Response& response);

// OS agnostic implementations
void TcpServerProcess();
sisyphus::Response TcpHandleRequest(sisyphus::Request& request);

#endif
