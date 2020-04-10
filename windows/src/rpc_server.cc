#include "rpc_server.h"

RpcServer::RpcServer() {}

const std::string RpcServer::HandleMessage(
    std::string incoming_message, Application* app) {
  return "";
}

const std::string RpcServer::HandleSetLedIntensity(
    std::string request_string, Application* app) {
  return "";
}

const std::string RpcServer::HandleGetLedIntensity(
    std::string request_string, Application* app) {
  return "";
}

const std::string RpcServer::HandleInsertPattern(
    std::string request_string, Application* app) {
  return "";
}

const std::string RpcServer::HandleDeletePattern(
    std::string request_string, Application* app) {
  return "";
}

const std::string RpcServer::HandleGetPatterns(
    std::string request_string, Application* app) {
  return "";
}
