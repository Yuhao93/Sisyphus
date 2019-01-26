#include "gpio.h"

#include "wiring_pi_wrapper.h"

void Gpio::setup() {
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_STEP, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_DIR, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_STEP, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_DIR, WIRING_PI_OUTPUT);
  pinMode(GPIO_LINEAR_FEEDBACK, WIRING_PI_INPUT);
}

void Gpio::write(int pin, int value) {
  digitalWrite(pin, value);
}

int Gpio::read(int pin) {
  return digitalRead(pin);
}
