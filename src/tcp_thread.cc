#include "tcp_thread.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sstream>
#include <stdlib.h>
#include <netinet/in.h>

namespace {
  int server_fd;
  bool socket_setup;
  sockaddr_in address;
  int addrlen = sizeof(address);
  std::string http_response_header = "HTTP/1.1 200 OK\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "\r\n";
}

void TcpThread::Init() {
  int opt = 1;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("Error creating socket\n");
    return;
  }
  // Forcefully attaching socket to the port 80
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    printf("Error setting socket options\n");
    return;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(80);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (sockaddr *) &address, sizeof(address)) < 0) {
    printf("Error binding socket\n");
    return;
  }
  if (listen(server_fd, 3) < 0) {
    printf("Error listening\n");
    return;
  }
  socket_setup = true;
}

void TcpThread::Run() {
  if (!socket_setup) {
    return;
  }
  int socket;
  if ((socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*) &addrlen)) < 0) {
    printf("ERROR accepting socket!\n");
    return;
  }

  std::ostringstream oss;
  char buffer[2048] = {0};
  int bytes_read = read(socket, buffer, 2048);
  while (bytes_read != 0) {
    oss.write(buffer, bytes_read);
  }
  std::string request = oss.str();
  std::size_t index = request.find("\r\n\r\n");
  std::string response = http_response_header;
  if (index != std::string::npos) {
    std::string payload = request.substr(index + 4);
    response += rpc_server_->HandleMessage(payload, app_);
  }
  send(socket, response.c_str(), strlen(response.c_str()), 0);
  close(socket);
}
