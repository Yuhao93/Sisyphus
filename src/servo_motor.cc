#include "servo_motor.h"

#include "sisyphus_util.h"
#include <wiringPi.h>
#include <cstdio>

namespace servo_motor_impl {
int min = 70;
int max = 238;
int sweep = max - min;

int stepsForServo(float angle) {
  return offset
    + ((angle / (2 * SisyphusUtil::pi)) * sweep);
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
