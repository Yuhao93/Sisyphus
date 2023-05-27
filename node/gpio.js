const {compatRequire} = require('./compat_require');
const rpio  = compatRequire('rpio');
const pwm = compatRequire('rpio-pwm');


const GPIO_STEPPER_MOTORS_ANGLE_STEP = 11;
const GPIO_STEPPER_MOTORS_ANGLE_DIR = 13;
const GPIO_STEPPER_MOTORS_ANGLE_MODE_1 = 19;
const GPIO_STEPPER_MOTORS_ANGLE_MODE_2 = 21;

const GPIO_STEPPER_MOTORS_LINEAR_STEP = 16;
const GPIO_STEPPER_MOTORS_LINEAR_DIR = 18;
const GPIO_STEPPER_MOTORS_LINEAR_MODE_1 = 24;
const GPIO_STEPPER_MOTORS_LINEAR_MODE_2 = 26;

const GPIO_LINEAR_FEEDBACK = 22;

/** NOTE: GPIO 22 -> Physical 15 */
const GPIO_LED = 22;
const GPIO_LED_RANGE = 100;
const GPIO_LED_PERIOD_US = 5000;

const linear_mode = [rpio.HIGH, rpio.HIGH];
const angle_mode = [rpio.HIGH, rpio.HIGH];

let pwmChannel;
let pwmPin;

function setup() {
  rpio.init({
		gpiomem: false
	});
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_STEP, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_DIR, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_STEP, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_DIR, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, rpio.OUTPUT);
  rpio.open(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, rpio.OUTPUT);
  rpio.open(GPIO_LINEAR_FEEDBACK, rpio.INPUT);
 
	pwmChannel = pwm.create_dma_channel(14, {
		cycle_time_us: GPIO_LED_PERIOD_US,
		step_time_us: GPIO_LED_PERIOD_US / GPIO_LED_RANGE,
	});
	pwmPin = pwmChannel.create_pwm(GPIO_LED);
 
  rpio.write(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, angle_mode[0]);
  rpio.write(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, angle_mode[1]);
  rpio.write(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, linear_mode[0]);
  rpio.write(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, linear_mode[1]);
}

function pwmWrite(pin, value) {
	pwmPin.set_width(value)
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
  
  GPIO_STEPPER_MOTORS_ANGLE_STEP,
  GPIO_STEPPER_MOTORS_ANGLE_DIR,
  GPIO_STEPPER_MOTORS_ANGLE_MODE_1,
  GPIO_STEPPER_MOTORS_ANGLE_MODE_2,
  
  GPIO_LINEAR_FEEDBACK,
  
  GPIO_LED,
  GPIO_LED_RANGE,
  HIGH: rpio.HIGH,
  LOW: rpio.LOW
};
