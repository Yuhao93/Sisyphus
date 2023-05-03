const gpio = require('./gpio');

let v;

function setIntensity(intensity) {
  v = Math.max(Math.min(1, intensity), 0);
  const valueInRange = Math.floor(gpio.GPIO_LED_RANGE * v);
  gpio.pwmWrite(valueInRange);
}

function getIntensity() {
  return v;
}

module.exports = { setIntensity, getIntensity };