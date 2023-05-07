const rpio = require('rpio');
const Gpio = require('./gpio');
const Model = require('./gen/model_pb');

function prepare(dirPin, movement) {
  if (movement == Model.Step.Movement.STOP) {
    return;
  }
  Gpio.write(dirPin, movement == Model.Step.Movement.FORWARDS ? Gpio.LOW : Gpio.HIGH);
}

function begin(stepPin, movement) {
  if (movement == Model.Step.Movement.STOP) {
    return;
  }
  Gpio.write(stepPin, Gpio.HIGH);
}

function end(stepPin, movement) {
  if (movement == Model.Step.Movement.STOP) {
    return;
  }
  Gpio.write(stepPin, Gpio.LOW);
}

function sleepMs(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

class StepperMotors {
  async step(step) {
    const angularMovement = step.getAngularMovement();
    const linearMovement = step.getLinearMovement();
    prepare(Gpio.GPIO_STEPPER_MOTORS_ANGLE_DIR, angularMovement);
    prepare(Gpio.GPIO_STEPPER_MOTORS_LINEAR_DIR, linearMovement);
    begin(Gpio.GPIO_STEPPER_MOTORS_ANGLE_STEP, angularMovement);
    begin(Gpio.GPIO_STEPPER_MOTORS_LINEAR_STEP, linearMovement);
    rpio.msleep(1);
		end(Gpio.GPIO_STEPPER_MOTORS_ANGLE_STEP, angularMovement);
    end(Gpio.GPIO_STEPPER_MOTORS_LINEAR_STEP, linearMovement);
  }
}

module.exports = StepperMotors;
