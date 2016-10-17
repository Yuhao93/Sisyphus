#include "pattern.h"

#include "servo_motor.h"
#include "sisyphus_util.h"
#include "stepper_motor.h"
#include "structs.h"
#include "wiringPi.h"
#include <cstdint>

Pattern::Pattern() {
  current_index = 0;
  wiringPiSetup();
  stepper_motor.setup();
  servo_motor.setup();
}

void Pattern::queuePattern(std::vector<ArmAngle> pattern) {
  points = pattern;
}

void Pattern::erase() {

}

void Pattern::step() {
  if (current_index >= points.size()) {
    return;
  }
  uint32_t start_time = millis();
  ArmAngle angle = points[current_index];
  bool keep_going =
      servo_motor.step(angle.servo_angle)
          && stepper_motor.step(angle.stepper_angle);
  while (!keep_going) {
    keep_going =
        servo_motor.step(angle.servo_angle)
            && stepper_motor.step(angle.stepper_angle);
  }
  uint32_t diff = millis() - start_time;
  if (diff < 100) {
    delay(100 - diff);
  }
  current_index++;
}
