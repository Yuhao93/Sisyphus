const gpio = require('./gpio');

let v;

function setIntensity(intensity) {
  v = intensity;
  const valueInRange = Math.floor(gpio.GPIO_LED_RANGE * intensity);
  gpio.pwmWrite(valueInRange);
}

function getIntensity() {
  return v;
}

module.exports = { setIntensity, getIntensity };