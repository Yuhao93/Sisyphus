#include "rpc_server.h"

#include "model.pb.h"
#include "server.pb.h"
#include <string>

RpcServer::RpcServer() {
  service_handlers["sisyphus.SisyphusService.SetLedIntensity"] =
      [=](std::string r) -> const std::string { return HandleSetLedIntensity(r); };
  service_handlers["sisyphus.SisyphusService.GetLedIntensity"] =
      [=](std::string r) -> const std::string { return HandleGetLedIntensity(r); };
  service_handlers["sisyphus.SisyphusService.InsertPattern"] =
      [=](std::string r) -> const std::string { return HandleInsertPattern(r); };
  service_handlers["sisyphus.SisyphusService.DeletePattern"] =
      [=](std::string r) -> const std::string { return HandleDeletePattern(r); };
  service_handlers["sisyphus.SisyphusService.GetPatterns"] =
      [=](std::string r) -> const std::string { return HandleGetPatterns(r); };
}

const std::string RpcServer::HandleMessage(std::string incoming_message) {
  int split_point = incoming_message.find(":");
  std::string signature = incoming_message.substr(0, split_point);
  std::string message_contents = incoming_message.substr(split_point + 1);

  auto service_handler = service_handlers.find(signature);
  if (service_handler == service_handlers.end()) {
    return "ERROR: No service handler found for " + signature + ";";
  }
  return signature + ";" + service_handler->second(message_contents);
}

const std::string RpcServer::HandleSetLedIntensity(std::string request_string) {
  sisyphus::SetLedIntensityRequest request;
  sisyphus::SetLedIntensityResponse response;
  request.ParseFromString(request_string);
  return response.SerializeAsString();
}

const std::string RpcServer::HandleGetLedIntensity(std::string request_string) {
  sisyphus::GetLedIntensityRequest request;
  sisyphus::GetLedIntensityResponse response;
  request.ParseFromString(request_string);
  return response.SerializeAsString();
}

const std::string RpcServer::HandleInsertPattern(std::string request_string) {
  sisyphus::InsertPatternRequest request;
  sisyphus::InsertPatternResponse response;
  request.ParseFromString(request_string);
  return response.SerializeAsString();
}

const std::string RpcServer::HandleDeletePattern(std::string request_string) {
  sisyphus::DeletePatternRequest request;
  sisyphus::DeletePatternResponse response;
  request.ParseFromString(request_string);
  return response.SerializeAsString();
}

const std::string RpcServer::HandleGetPatterns(std::string request_string) {
  sisyphus::GetPatternsRequest request;
  sisyphus::GetPatternsResponse response;
  request.ParseFromString(request_string);
  return response.SerializeAsString();
}
