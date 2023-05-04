const Model = require('./gen/model_pb');
const Server = require('./gen/server_pb');
const Led = require('./led');
const SisyphusUtil = require('./sisyphus_util');
const Firebase = require('./firebase');

async function handleSetLedIntesity(request, patternManager) {
  Led.setIntensity(request.getIntensity());
  return new Server.SetLedIntensityResponse();
}

async function handleGetLedIntensity(request, patternManager) {
  const response = new Server.GetLedIntensityResponse();
  response.setIntensity(Led.getIntensity());
  return response;
}

async function handleInsertPattern(request, patternManager) {
	const id = request.getPattern();
	const patternB64 = await Firebase.getPattern(id);
	const patternBuffer = Buffer.from(patternB64, 'base64');
	const storedPattern = Model.StoredPattern.deserializeBinary(patternBuffer);	

  patternManager.queuePattern(
      SisyphusUtil.storedPatternToPattern(storedPattern), id);
  return new Server.InsertPatternResponse();
}

async function handleDeletePattern(request, patternManager) {
  return new Server.DeletePatternResponse();
}

async function handleGetPatterns(request, patternManager) {
  const response = new Server.GetPatternsResponse();
  for (const pattern of patternManager.listPatterns()) {
		response.addUpcomingPatterns(pattern.id);
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

async function handle(signature, requestBinary, patternManager) {
  const handler = serviceMap[signature];
  if (!handler) {
    return new Uint8Array([1]);
  }
  const request = handler[0].deserializeBinary(requestBinary);
  const response = await handler[1](request, patternManager);
  return response.serializeBinary();
}

module.exports = {handle};
