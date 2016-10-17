#include "stepper_motor.h"

#include "sisyphus_util.h"
#include <wiringPi.h>
#include <cmath>
#include <cstdio>

void StepperMotor::setup() {
  pinMode(STEPPER_MOTOR_GPIO_STEP, OUTPUT);
  pinMode(STEPPER_MOTOR_GPIO_DIR, OUTPUT);
}

bool StepperMotor::step(float target) {
  target_angle = SisyphusUtil::ClampBetween2Pi(target);
  float angle_to_step =
      SisyphusUtil::DiffBetweenAngles(target_angle, current_angle);
  if (fabs(angle_to_step) > STEPPER_MOTOR_HALF_STEP_ANGLE) {
    if (angle_to_step > 0) {
      digitalWrite(STEPPER_MOTOR_GPIO_DIR, HIGH);
      current_angle =
          SisyphusUtil::ClampBetween2Pi(
              current_angle + STEPPER_MOTOR_STEP_ANGLE);
    } else {
      digitalWrite(STEPPER_MOTOR_GPIO_DIR, LOW);
      current_angle =
          SisyphusUtil::ClampBetween2Pi(
              current_angle - STEPPER_MOTOR_STEP_ANGLE);
    }
    delay(1);
    digitalWrite(STEPPER_MOTOR_GPIO_STEP, HIGH);
    delay(1);
    digitalWrite(STEPPER_MOTOR_GPIO_STEP, LOW);
    float new_diff_angle =
        fabs(SisyphusUtil::DiffBetweenAngles(target_angle, current_angle));
    return new_diff_angle <= STEPPER_MOTOR_HALF_STEP_ANGLE;
  }
  return true;
}
