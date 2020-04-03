#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include "application.h"
#include <string>
#include <map>
#include <functional>

class RpcServer {
public:
  RpcServer();
  const std::string HandleMessage(
    std::string incoming_message, Application* app);

private:
  const std::string HandleSetLedIntensity(std::string request_string, Application* app);
  const std::string HandleGetLedIntensity(std::string request_string, Application* app);
  const std::string HandleInsertPattern(std::string request_string, Application* app);
  const std::string HandleDeletePattern(std::string request_string, Application* app);
  const std::string HandleGetPatterns(std::string request_string, Application* app);

  std::map<std::string, std::function<const std::string(std::string)>> service_handlers;
};

#endif
