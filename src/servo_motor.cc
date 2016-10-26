#include "servo_motor.h"

#include "sisyphus_util.h"
#include <wiringPi.h>
#include <cstdio>

namespace servo_motor_impl {
int stepsPerMillis = 100;
float millisSweep = 1.7f;
int offset = stepsPerMillis * .7f;

int stepsForServo(float angle) {
  return offset
    + ((angle / (2 * SisyphusUtil::pi)) * stepsPerMillis * millisSweep);
}
}

void ServoMotor::setup() {
  pinMode(SERVO_MOTOR_GPIO_PWM, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(192);
  pwmSetRange(2000);
}

void ServoMotor::moveToStart(float start) {
  pwmWrite(SERVO_MOTOR_GPIO_PWM, servo_motor_impl::stepsForServo(start));
}

bool ServoMotor::step(float target_angle) {
  pwmWrite(
      SERVO_MOTOR_GPIO_PWM,
      servo_motor_impl::stepsForServo(target_angle));
  return true;
}
