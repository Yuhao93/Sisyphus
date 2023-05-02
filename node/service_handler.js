const Server = require('./gen/server_pb');
const Led = require('./led');
const SisyphusUtil = require('./sisyphus_util');

function handleSetLedIntesity(request, patternManager) {
  Led.setIntensity(request.getIntensity());
  return new Server.SetLedIntensityResponse();
}

function handleGetLedIntensity(request, patternManager) {
  const response = new Server.GetLedIntensityResponse();
  response.setIntensity(Led.getIntensity());
  return response;
}

function handleInsertPattern(request, patternManager) {
  patternManager.queuePattern(
      SisyphusUtil.storedPatternToPattern(request.getPattern()));
  return new Server.InsertPatternResponse();
}

function handleDeletePattern(request, patternManager) {
  return new Server.DeletePatternResponse();
}

function handleGetPatterns(request, patternManager) {
  const response = new Server.GetPatternsResponse();
  for (const pattern of patternManager.listPatterns()) {
    response.addUpcomingPatterns(SisyphusUtil.patternToPolarStoredPattern(pattern));
  }
  return response;
}

const serviceMap = {
  'sisyphus.SisyphusService.SetLedIntensity':
      [Server.SetLedIntensityRequest, handleSetLedIntesity],
  'sisyphus.SisyphusService.GetLedIntensity':
      [Server.GetLedIntensityRequest, handleGetLedIntensity],
  'sisyphus.SisyphusService.InsertPattern':
      [Server.InsertPatternRequest, handleInsertPattern],
  'sisyphus.SisyphusService.DeletePattern':
      [Server.DeletePatternRequest, handleDeletePattern],
  'sisyphus.SisyphusService.GetPatterns':
      [Server.GetPatternsRequest, handleGetPatterns],
};

function handle(signature, requestBinary, patternManager) {
  const handler = serviceMap[signature];
  if (!handler) {
    return new Uint8Array([1]);
  }
  const request = handler[0].deserializeBinary(requestBinary);
  const response = handler[1](request, patternManager);
  return response.serializeBinary();
}

module.exports = {handle};
