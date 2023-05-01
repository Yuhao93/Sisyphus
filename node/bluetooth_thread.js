const bleno = require('bleno');
const ServiceHandler = require('./service_handler');

const name = 'Sisyphus Table';
const serviceUuid = '6d081fa1965e48e79aec2d122334954b';
let p = null;
let currentRequest = null;
const poweredOn = new Promise((resolve, reject) => {
	bleno.on('stateChange', (state) => {
		console.log('Bluetooth State Changed: ' + state);
		if (state == 'poweredOn') {
			resolve();
		}
	});
});

function descriptorName(name) {
  return new bleno.Descriptor({
    uuid: '2901',
    value: name
  });
}

async function onWriteRequest(data, offset, withoutResponse, callback) {
	console.log('Bluetooth received Write');
  if (currentRequest != null) {
    await currentRequest;
  }
  const request = JSON.parse(data.toString('utf8'));
  currentRequest = new Promise((resolve, reject) => {
    resolve(ServiceHandler.handle(request.type, request.payload, p));
  });
  callback(bleno.Characteristic.RESULT_SUCCESS);
}

async function onSubscribe(maxValueSize, updateValueCallback) {
	console.log('Bluetooth received Subscribe');
  if (currentRequest == null) {
    return;
  }
  updateValueCallback(await currentRequest);
  currentRequest = null;
}

async function initializeBluetooth(patternManager) {
	await poweredOn;
  p = patternManager;
  const rpcCharacteristic = new bleno.Characteristic({
      uuid: 'fff0',
      properties: [ 'notify', 'write' ],
      descriptors: [ descriptorName('TableConnection') ],
      onSubscribe,
      onWriteRequest
  });

  const primaryService = new bleno.PrimaryService({
      uuid: serviceUuid,
      characteristics: [ rpcCharacteristic ]
  });

  bleno.setServices([primaryService]);
  bleno.startAdvertising(name, [serviceUuid]);
	console.log('Bluetooth Advertising Started');
}

module.exports = { initializeBluetooth };
