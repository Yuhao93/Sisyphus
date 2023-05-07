const PatternManager = require('./pattern_manager');
const BluetoothThread = require('./bluetooth_thread');

const patternManager = new PatternManager();
BluetoothThread.initializeBluetooth(patternManager);

async function doPatternLoop() {
  patternManager.step();
  setImmediate(doPatternLoop);
}

doPatternLoop();
