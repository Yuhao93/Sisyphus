#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#define STEPPER_MOTOR_STEP_ANGLE (2 * 3.14159f * 1.8f / 360.0f)
#define STEPPER_MOTOR_HALF_STEP_ANGLE (STEPPER_MOTOR_STEP_ANGLE / 2)
#define STEPPER_MOTOR_GPIO_STEP 0
#define STEPPER_MOTOR_GPIO_DIR 2

class StepperMotor {
public:
  StepperMotor() : current_angle(0) {}

  /** intialize pins */
  void setup();

  /** Move to start angle. */
  void moveToStart(float start);

  /**
   * Move target_angle radians (+ for colckwise, - for counter clockwise)
   * Return true if no more movement is needed, false if it needs to keep
   * moving
   */
  bool step(float target);

private:
  float current_angle;
};

#endif
