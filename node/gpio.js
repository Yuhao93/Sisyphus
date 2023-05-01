const rpio  = require('rpio');

const GPIO_STEPPER_MOTORS_LINEAR_STEP = 0;
const GPIO_STEPPER_MOTORS_LINEAR_DIR = 2;
const GPIO_STEPPER_MOTORS_LINEAR_MODE_1 = 12;
const GPIO_STEPPER_MOTORS_LINEAR_MODE_2 = 13;
const GPIO_STEPPER_MOTORS_LINEAR_MODE_3 = 14;

const GPIO_STEPPER_MOTORS_ANGLE_STEP = 4;
const GPIO_STEPPER_MOTORS_ANGLE_DIR = 5;
const GPIO_STEPPER_MOTORS_ANGLE_MODE_1 = 10;
const GPIO_STEPPER_MOTORS_ANGLE_MODE_2 = 11;
const GPIO_STEPPER_MOTORS_ANGLE_MODE_3 = 31;

const GPIO_LINEAR_FEEDBACK = 6;

const GPIO_LED = 3;
const GPIO_LED_RANGE = 100;

const linear_mode = [rpio.HIGH, rpio.HIGH, rpio.HIGH];
const angle_mode = [rpio.HIGH, rpio.HIGH, rpio.HIGH];

function setup() {
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_STEP, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_DIR, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_MODE_3, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_STEP, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_DIR, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_MODE_3, rpio.OUTPUT);
  rpio.open(GPIO_LINEAR_FEEDBACK, rpio.INPUT);
  rpio.open(GPIO_LED, rpio.PWM)
  rpio.pwmSetClockDivider(64);
  rpio.pwmSetRange(0, GPIO_LED_RANGE);

  rpio.write(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, angle_mode[0]);
  rpio.write(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, angle_mode[1]);
  rpio.write(GPIO_STEPPER_MOTORS_ANGLE_MODE_3, angle_mode[2]);
  rpio.write(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, linear_mode[0]);
  rpio.write(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, linear_mode[1]);
  rpio.write(GPIO_STEPPER_MOTORS_LINEAR_MODE_3, linear_mode[2]);
}

function pwmWrite(pin, value) {
  rpio.pwmSetData(pin, value);
}

function write(pin, value) {
  rpio.write(pin, value);
}

function read(pin) {
  return rpio.read(pin);
}

module.exports = {
  setup,
  pwmWrite,
  write,
  read,
  GPIO_STEPPER_MOTORS_LINEAR_STEP,
  GPIO_STEPPER_MOTORS_LINEAR_DIR,
  GPIO_STEPPER_MOTORS_LINEAR_MODE_1,
  GPIO_STEPPER_MOTORS_LINEAR_MODE_2,
  GPIO_STEPPER_MOTORS_LINEAR_MODE_3,
  
  GPIO_STEPPER_MOTORS_ANGLE_STEP,
  GPIO_STEPPER_MOTORS_ANGLE_DIR,
  GPIO_STEPPER_MOTORS_ANGLE_MODE_1,
  GPIO_STEPPER_MOTORS_ANGLE_MODE_2,
  GPIO_STEPPER_MOTORS_ANGLE_MODE_3,
  
  GPIO_LINEAR_FEEDBACK,
  
  GPIO_LED,
  GPIO_LED_RANGE,
  HIGH: rpio.HIGH,
  LOW: rpio.LOW
};