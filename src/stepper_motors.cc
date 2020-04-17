#include "stepper_motors.h"

#include "gpio.h"
#include "model.pb.h"
#include "wiring_pi_wrapper.h"
#include <cstdio>
#include <string>

namespace {

void prepare(int dir_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  Gpio::write(dir_pin, movement == sisyphus::Step::FORWARDS ? LOW : HIGH);
}

void begin(int step_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  Gpio::write(step_pin, HIGH);
}

void end(int step_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  Gpio::write(step_pin, LOW);
}

}

void StepperMotors::step(const sisyphus::Step& step) {
  sisyphus::Step_Movement angular_movement = step.angular_movement();
  sisyphus::Step_Movement linear_movement = step.linear_movement();
  prepare(GPIO_STEPPER_MOTORS_ANGLE_DIR, angular_movement);
  prepare(GPIO_STEPPER_MOTORS_LINEAR_DIR, linear_movement);
  begin(GPIO_STEPPER_MOTORS_ANGLE_STEP, angular_movement);
  begin(GPIO_STEPPER_MOTORS_LINEAR_STEP, linear_movement);
  delayMicroseconds(1000);
  end(GPIO_STEPPER_MOTORS_ANGLE_STEP, angular_movement);
  end(GPIO_STEPPER_MOTORS_LINEAR_STEP, linear_movement);
}
