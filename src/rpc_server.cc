#include "rpc_server.h"

#include "application.h"
#include "model.pb.h"
#include "server.pb.h"
#include "sisyphus_util.h"
#include <google/protobuf/text_format.h>
#include <string>

using google::protobuf::TextFormat;

RpcServer::RpcServer() {
  service_handlers["sisyphus.SisyphusService.SetLedIntensity"] =
      [=](std::string r, Application* a) -> const std::string {
        return HandleSetLedIntensity(r, a);
      };
  service_handlers["sisyphus.SisyphusService.GetLedIntensity"] =
      [=](std::string r, Application* a) -> const std::string {
        return HandleGetLedIntensity(r, a);
      };
  service_handlers["sisyphus.SisyphusService.InsertPattern"] =
      [=](std::string r, Application* a) -> const std::string {
        return HandleInsertPattern(r, a);
      };
  service_handlers["sisyphus.SisyphusService.DeletePattern"] =
      [=](std::string r, Application* a) -> const std::string {
        return HandleDeletePattern(r, a);
      };
  service_handlers["sisyphus.SisyphusService.GetPatterns"] =
      [=](std::string r, Application* a) -> const std::string {
        return HandleGetPatterns(r, a);
      };
}

const std::string RpcServer::HandleMessage(
    std::string incoming_message, Application* app) {
  int split_point = incoming_message.find(";");
  std::string signature = incoming_message.substr(0, split_point);
  std::string message_contents = incoming_message.substr(split_point + 1);

  auto service_handler = service_handlers.find(signature);
  if (service_handler == service_handlers.end()) {
    return "ERROR: No service handler found for " + signature + ";";
  }
  return signature + ";" + service_handler->second(message_contents, app);
}

const std::string RpcServer::HandleSetLedIntensity(
    std::string request_string, Application* app) {
  sisyphus::SetLedIntensityRequest request;
  sisyphus::SetLedIntensityResponse response;
  TextFormat::ParseFromString(request_string, &request);
  app->led.setIntensity(request.intensity());
  return response.DebugString();
}

const std::string RpcServer::HandleGetLedIntensity(
    std::string request_string, Application* app) {
  sisyphus::GetLedIntensityRequest request;
  sisyphus::GetLedIntensityResponse response;
  TextFormat::ParseFromString(request_string, &request);
  response.set_intensity(app->led.getIntensity());
  return response.DebugString();
}

const std::string RpcServer::HandleInsertPattern(
    std::string request_string, Application* app) {
  sisyphus::InsertPatternRequest request;
  sisyphus::InsertPatternResponse response;
  TextFormat::ParseFromString(request_string, &request);
  app->pattern_manager.QueuePattern(SisyphusUtil::StoredPatternToPattern(request.pattern()));
  return response.DebugString();
}

const std::string RpcServer::HandleDeletePattern(
    std::string request_string, Application* app) {
  sisyphus::DeletePatternRequest request;
  sisyphus::DeletePatternResponse response;
  TextFormat::ParseFromString(request_string, &request);
  return response.DebugString();
}

const std::string RpcServer::HandleGetPatterns(
    std::string request_string, Application* app) {
  sisyphus::GetPatternsRequest request;
  sisyphus::GetPatternsResponse response;
  TextFormat::ParseFromString(request_string, &request);
  for (const auto& pattern : app->pattern_manager.ListPatterns()) {
    *response.add_upcoming_patterns() = SisyphusUtil::PatternToPolarStoredPattern(pattern);
  }
  return response.DebugString();
}
