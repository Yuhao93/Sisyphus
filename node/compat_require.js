const os = require('./os');

class BlenoCharacteristic {
  constructor(obj) {}

  static RESULT_SUCCESS = 1;
}

class BlenoDescriptor {
  constructor(obj) {}
}

class BlenoPrimaryService {
  constructor(obj) {}
}

const windowsBleno = {
  on: function() {},
  setServices: function() {},
  startAdvertising: function() {},
  Characteristic: BlenoCharacteristic,
  Descriptor: BlenoDescriptor,
  PrimaryService: BlenoPrimaryService
};

const windowsRpio = {
  msleep: function() {},
  init: function() {},
  open: function() {},
  write: function() {},
  read: function() {
    return 0;
  }
};

const windowsRpioPwm = {
  create_dma_channel: function() {
    return {
      create_pwm: function() {},
      set_width: function() {}
    };
  }
};

const compatMap = {
  'bleno': windowsBleno,
  'rpio': windowsRpio,
  'rpio-pwm': windowsRpioPwm
};

function compatRequire(pkg) {
  if (compatMap[pkg] && os.isWindows()) {
    return compatMap[pkg];
  }
  return require(pkg);
}

module.exports = {
  compatRequire
};