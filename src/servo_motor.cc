#include "servo_motor.h"

#include "sisyphus_util.h"
#include <wiringPi.h>
#include <cstdio>

namespace servo_motor_impl {
int stepsPerMillis = 100;
float millisSweep = 1.8f;
int offset = stepsPerMillis * .6f;

int stepsForServo(float angle) {
  return offset + ((angle / SisyphusUtil::pi) * stepsPerMillis * millisSweep);
}
}

void ServoMotor::setup() {
  pinMode(SERVO_MOTOR_GPIO_PWM, PWM_OUTPUT);
  pwmSetMode(PWM_MODE_MS);
  pwmSetClock(192);
  pwmSetRange(2000);
}

bool ServoMotor::step(float target_angle) {
  pwmWrite(
      SERVO_MOTOR_GPIO_PWM,
      servo_motor_impl::stepsForServo(target_angle));
  return true;
}
