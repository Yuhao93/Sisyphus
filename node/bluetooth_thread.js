const bleno = require('bleno');
const ServiceHandler = require('./service_handler');

const name = 'Sisyphus Table';
const serviceUuid = '6d081fa1965e48e79aec2d122334954b';
const textEncoder = new TextEncoder();

let p = null;
let resultCallback = null;
let clearWrites = [];

let writeAuthor = null;
let writeComplete = false;
let writeBuffer = '';

let readComplete = false;
let readBuffer = null;

function createResultPromise() {
  return new Promise((resolve, reject) => {
    resultCallback = resolve;
  });
}

function readyForWritePromise() {
  return new Promise((resolve, reject) => {
    clearWrites.push(resolve);
  });
}

let result = createResultPromise();
let pendingWrite = null;

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

  const author = new Uint8Array(data, 0, 4);

  if (!writeComplete && writeAuthor != null && writeAuthor.toString() != author.toString()) {
    const currentPendingWrite = pendingWrite;
    pendingWrite = readyForWritePromise();
    if (currentPendingWrite != null) {
      await currentPendingWrite;
    }
  }

  writeComplete = false;
  writeAuthor = author;
  const complete = String.fromCharCode(new Uint8Array(data, 4, 1)[0]) == '1';
  writeBuffer += textEncoder.decode(new Uint8Array(data, 5));

  if (complete) {
    writeComplete = true;
    const request = JSON.parse(writeBuffer);
    const payload = Buffer.from(request.payload, 'base64');
    ServiceHandler.handle(request.type, payload, p);
    resultCallback(content);
    callback(bleno.Characteristic.RESULT_SUCCESS);
  }
}

async function onReadRequest() {
	console.log('Bluetooth received Read');

  const data = await result;
  callback = null;
  result = createResultPromise();
  clearWrites.shift()();
  pendingWrite = null;
  return data;
}

async function initializeBluetooth(patternManager) {
	await poweredOn;
  p = patternManager;

  const rpcRequestCharacteristic = new bleno.Characteristic({
    uuid: 'fff0',
    properties: ['write'],
    descriptors: [ descriptorName('Send') ],
    onWriteRequest
  });

  const rpcResponseCharacteristic = new bleno.Characteristic({
    uuid: 'fff1',
    properties: ['read'],
    descriptors: [ descriptorName('Read') ],
    onReadRequest
  });

  const primaryService = new bleno.PrimaryService({
    uuid: serviceUuid,
    characteristics: [
      rpcRequestCharacteristic,
      rpcResponseCharacteristic
    ]
  });

  bleno.setServices([primaryService]);
  bleno.startAdvertising(name, [serviceUuid]);
	console.log('Bluetooth Advertising Started');
}

module.exports = { initializeBluetooth };
