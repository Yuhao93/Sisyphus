#ifndef RPC_SERVER_H
#define RPC_SERVER_H

#include <string>
#include <map>
#include <functional>

class RpcServer {
public:
  RpcServer();

private:
  const std::string HandleMessage(std::string incoming_message);
  const std::string HandleSetLedIntensity(std::string request_string);
  const std::string HandleGetLedIntensity(std::string request_string);
  const std::string HandleInsertPattern(std::string request_string);
  const std::string HandleDeletePattern(std::string request_string);
  const std::string HandleGetPatterns(std::string request_string);

  std::map<std::string, std::function<const std::string(std::string)>> service_handlers;
};

#endif
