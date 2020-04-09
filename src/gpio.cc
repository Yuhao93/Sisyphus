#include "gpio.h"

#include "wiring_pi_wrapper.h"
#include <softPwm.h>
#include <cstdio>

int linear_mode[] = {HIGH, HIGH, HIGH};
int angle_mode[] = {HIGH, HIGH, HIGH};

void Gpio::setup() {
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_STEP, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_DIR, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_ANGLE_MODE_3, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_STEP, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_DIR, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, WIRING_PI_OUTPUT);
  pinMode(GPIO_STEPPER_MOTORS_LINEAR_MODE_3, WIRING_PI_OUTPUT);
  pinMode(GPIO_LINEAR_FEEDBACK, WIRING_PI_INPUT);
  softPwmCreate(GPIO_LED, 0, GPIO_LED_RANGE);

  digitalWrite(GPIO_STEPPER_MOTORS_ANGLE_MODE_1, angle_mode[0]);
  digitalWrite(GPIO_STEPPER_MOTORS_ANGLE_MODE_2, angle_mode[1]);
  digitalWrite(GPIO_STEPPER_MOTORS_ANGLE_MODE_3, angle_mode[2]);
  digitalWrite(GPIO_STEPPER_MOTORS_LINEAR_MODE_1, linear_mode[0]);
  digitalWrite(GPIO_STEPPER_MOTORS_LINEAR_MODE_2, linear_mode[1]);
  digitalWrite(GPIO_STEPPER_MOTORS_LINEAR_MODE_3, linear_mode[2]);
}

void Gpio::pwmWrite(int pin, int value) {
  softPwmWrite(pin, value); 
}

void Gpio::write(int pin, int value) {
  digitalWrite(pin, value);
}

int Gpio::read(int pin) {
  return digitalRead(pin);
}
