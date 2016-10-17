#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#define SERVO_MOTOR_STEP_ANGLE 1f
#define SERVO_MOTOR_HALF_STEP_ANGLE (SERVO_MOTOR_STEP_ANGLE / 2)
#define SERVO_MOTOR_GPIO_PWM 1

class ServoMotor {
public:
  /** intialize pins */
  void setup();

  /**
   * Move to target_angle in radians (+ for colckwise, - for counter clockwise)
   * Return true if no more movement is needed, false if it needs to keep
   * moving
   */
  bool step(float target);
};

#endif
