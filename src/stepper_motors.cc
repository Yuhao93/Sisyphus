#include "stepper_motors.h"

#include "model.pb.h"
#include "wiring_pi_wrapper.h"
#include <cstdio>

namespace {

void prepare(int dir_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  digitalWrite(dir_pin, movement == sisyphus::Step::FORWARDS ? HIGH : LOW);
}

void begin(int step_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  digitalWrite(step_pin, HIGH);
}

void end(int step_pin, const sisyphus::Step_Movement& movement) {
  if (movement == sisyphus::Step::STOP) {
    return;
  }
  digitalWrite(step_pin, LOW);
}

}

void StepperMotors::setup() {
  pinMode(STEPPER_MOTORS_GPIO_ANGLE_STEP, WIRING_PI_OUTPUT);
  pinMode(STEPPER_MOTORS_GPIO_ANGLE_DIR, WIRING_PI_OUTPUT);
  pinMode(STEPPER_MOTORS_GPIO_LINEAR_STEP, WIRING_PI_OUTPUT);
  pinMode(STEPPER_MOTORS_GPIO_LINEAR_DIR, WIRING_PI_OUTPUT);
  pinMode(STEPPER_MOTORS_GPIO_FEEDBACK, WIRING_PI_INPUT);
}

void StepperMotors::move_to_start(float magnitude) {
  // First move to the origin
  sisyphus::Step linear_back;
  linear_back.set_angular_movement(sisyphus::Step::STOP);
  linear_back.set_linear_movement(sisyphus::Step::BACKWORDS);
  sisyphus::Step linear_forward;
  linear_forward.set_angular_movement(sisyphus::Step::STOP);
  linear_forward.set_linear_movement(sisyphus::Step::FORWARDS);
  while (digitalRead(STEPPER_MOTORS_GPIO_FEEDBACK) != HIGH) {
    step(linear_back);
    delay(5);
  }
  current_magnitude = 0;
  int num_steps = (int) (magnitude * STEPPER_MOTORS_LINEAR_STEPS_RANGE);
  for (int i = 0; i < num_steps; i++) {
    step(linear_forward);
    delay(5);
  }
}

void StepperMotors::step(const sisyphus::Step& step) {
  if (step.angular_movement() != sisyphus::Step::STOP) {
    current_angle +=
        step.angular_movement() == sisyphus::Step::FORWARDS ? 1 : -1;
    if (current_angle < 0) {
      current_angle = STEPPER_MOTORS_ANGULAR_STEPS_RANGE - 1;
    } else if (current_angle >= STEPPER_MOTORS_ANGULAR_STEPS_RANGE) {
      current_angle = 0;
    }
  }
  if (step.linear_movement() != sisyphus::Step::STOP) {
    current_magnitude +=
        step.linear_movement() == sisyphus::Step::FORWARDS ? 1 : -1;
  }
  sisyphus::Step_Movement angular_movement = step.angular_movement();
  sisyphus::Step_Movement linear_movement = step.linear_movement();
  prepare(STEPPER_MOTORS_GPIO_ANGLE_DIR, angular_movement);
  prepare(STEPPER_MOTORS_GPIO_LINEAR_DIR, linear_movement);
  delay(1);
  begin(STEPPER_MOTORS_GPIO_ANGLE_STEP, angular_movement);
  begin(STEPPER_MOTORS_GPIO_LINEAR_STEP, linear_movement);
  delay(1);
  end(STEPPER_MOTORS_GPIO_ANGLE_STEP, angular_movement);
  end(STEPPER_MOTORS_GPIO_LINEAR_STEP, linear_movement);
}
