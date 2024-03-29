const {compatRequire} = require('./compat_require');
const bleno = compatRequire('bleno');
const ServiceHandler = require('./service_handler');
const Server = require('./gen/server_pb');
const os = require('./os');
const rl = require("readline").createInterface({
  input: process.stdin,
  output: process.stdout
});

const name = 'Sisyphus Table';
const serviceUuid = '6d081fa1965e48e79aec2d122334954b';

let p = null;
let resultCallback = null;
let clearWrites = [];

let writeAuthor = null;
let writeComplete = false;
let writeBuffer = '';

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

	const currentPendingWrite = pendingWrite;
  const author = data.readInt32LE();
	console.log(author);
	if (writeAuthor == null || writeAuthor != author) {
		pendingWrite = readyForWritePromise();
	}
  if (writeAuthor != null && writeAuthor != author) {
    if (currentPendingWrite != null) {
      await currentPendingWrite;
    }
  }

  writeComplete = false;
  writeAuthor = author;
  const complete = String.fromCharCode(data.readUint8(4)) == '1';
	writeBuffer += data.subarray(5).toString('utf8');
	console.log(writeBuffer);

  if (complete) {
    writeComplete = true;
    const request = JSON.parse(writeBuffer);
    writeBuffer = '';
		const payload = Buffer.from(request.payload, 'base64');
    const content = await ServiceHandler.handle(request.type, payload, p);
    resultCallback(content);
	}
  callback(bleno.Characteristic.RESULT_SUCCESS);
}

async function onReadRequest(offset, callback) {
	console.log('Bluetooth received Read');

  const data = await result;
  result = createResultPromise();
  clearWrites.shift()();
  pendingWrite = null;
	console.log('Bluetooth read complete ' + data.length + 'bytes');
	callback(bleno.Characteristic.RESULT_SUCCESS, data);
}

async function initializeBluetooth(patternManager) {
  p = patternManager;

  if (os.isWindows()) {
    fakeRpc();
  }
	await poweredOn;

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

function fakeRpc() {
  rl.question('Pattern id to insert:', async (id) => {
    console.log(id);
    const req = new Server.InsertPatternRequest();
    req.setPattern(id);
    await ServiceHandler.handle('sisyphus.SisyphusService.InsertPattern', req.serializeBinary(), p);
  });
}

module.exports = { initializeBluetooth };