#include "tcp_thread.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <iostream>

namespace {
  int server_fd;
  bool socket_setup;
  sockaddr_in address;
  int addrlen = sizeof(address);
  std::string http_response_header = "HTTP/1.1 200 OK\r\n"
      "Access-Control-Allow-Origin: *\r\n"
      "Access-Control-Allow-Private-Network: true\r\n"
      "\r\n";
}

bool TooLong(std::chrono::time_point<std::chrono::system_clock> timestamp) {
  auto now = std::chrono::system_clock::now();
  std::chrono::duration<double> seconds = now - timestamp;
  return seconds.count() > 60;
}

std::string GetPayload(int socket) {
  auto timestamp = std::chrono::system_clock::now();
  char buffer[2048];
  std::ostringstream oss;
  std::string current_string;
  int bytes_read;
  std::string content_length_search = "Content-Length: ";
  do {
    if (TooLong(timestamp)) {
      printf("Could not find Content-Length\n");
      return "";  
    }
    bytes_read = read(socket, buffer, 2048);
    oss.write(buffer, bytes_read);
    current_string = oss.str();
  } while (current_string.find(content_length_search) == std::string::npos);
  int content_length_index = current_string.find(content_length_search);
  int start_of_content_length = content_length_index + content_length_search.length();
  int end_of_content_length = current_string.find("\r\n", content_length_index);
  std::string content_length_string = current_string.substr(start_of_content_length, end_of_content_length - start_of_content_length);
  int content_length = atoi(content_length_string.c_str());
  printf("Content-Length is %d\n", content_length);
  while (current_string.find("\r\n\r\n") == std::string::npos) {
    if (TooLong(timestamp)) {
      printf("Could not find Content start\n");
      return "";  
    }
    bytes_read = read(socket, buffer, 2048);
    oss.write(buffer, bytes_read);
    current_string = oss.str();
  }

  int content_start = current_string.find("\r\n\r\n") + 4;
  while (content_start + content_length > current_string.length()) {
    if (TooLong(timestamp)) {
      printf("Could not finish reading Content\n");
      return "";  
    }
    bytes_read = read(socket, buffer, 2048);
    oss.write(buffer, bytes_read);
    current_string = oss.str();
  }
  return current_string.substr(content_start, content_length);
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

  std::string payload = GetPayload(socket);
  std::string response = http_response_header;
  if (payload.length() > 0) {
    response += rpc_server_->HandleMessage(payload, app_);
  }
  send(socket, response.c_str(), strlen(response.c_str()), 0);
  close(socket);
}
