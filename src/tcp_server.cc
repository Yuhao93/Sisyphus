#include "tcp_server.h"

#include "tcp.pb.h"

sisyphus::GetResponse tcpHandleGetRequest(const sisyphus::GetRequest& request);
sisyphus::GetPatternsResponse tcpGetPatterns();
sisyphus::GetIntensityResponse tcpGetIntensity();
sisyphus::SetResponse tcpHandleSetRequest(const sisyphus::SetRequest& request);

void TcpServerProcess() {
  sisyphus::Request req = TcpNextRequest();
  sisyphus::Response resp = TcpHandleRequest(req);
  TcpWriteResponse(resp);
}

sisyphus::Response TcpHandleRequest(sisyphus::Request& request) {
  sisyphus::Response resp;
  if (request.has_get()) {
    resp.mutable_get()->CopyFrom(tcpHandleGetRequest(request.get()));
  }
  if (request.has_set()) {
    resp.mutable_set()->CopyFrom(tcpHandleGetRequest(request.get()));
  }
  return resp;
}

sisyphus::GetResponse tcpHandleGetRequest(const sisyphus::GetRequest& request) {
  sisyphus::GetResponse resp;
  if (request.get_patterns()) {
    resp.mutable_patterns()->CopyFrom(tcpGetPatterns());
  }
  if (request.get_intensity()) {
    resp.mutable_intensity()->CopyFrom(tcpGetIntensity());
  }
  return resp;
}

sisyphus::GetPatternsResponse tcpGetPatterns() {
  sisyphus::GetPatternsResponse resp;
  return resp;
}

sisyphus::GetIntensityResponse tcpGetIntensity() {
  sisyphus::GetIntensityResponse resp;
  return resp;
}

sisyphus::SetResponse tcpHandleSetRequest(const sisyphus::SetRequest& request) {
  sisyphus::SetResponse resp;
  if (request.has_add_pattern()) {

  }
  if (request.has_remove_pattern()) {

  }
  if (request.has_intensity()) {

  }
  return resp;
}
