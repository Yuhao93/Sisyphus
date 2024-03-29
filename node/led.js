const gpio = require('./gpio');

let v;

function setIntensity(intensity) {
  v = Math.max(Math.min(1, intensity), 0);
  const valueInRange = Math.floor(gpio.GPIO_LED_RANGE * (1 - v));
  gpio.pwmWrite(gpio.GPIO_LED, valueInRange);
}

function getIntensity() {
  return v;
}

module.exports = { setIntensity, getIntensity };
