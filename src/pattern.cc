#include "pattern.h"

#include "servo_motor.h"
#include "sisyphus_util.h"
#include "stepper_motor.h"
#include "structs.h"
#include "wiringPi.h"
#include <cstdint>

Pattern::Pattern() {
  current_index = 0;
  current_pattern_index = 0;
  wiringPiSetup();
  stepper_motor.setup();
  servo_motor.setup();
}

void Pattern::queuePattern(std::vector<ArmAngle> pattern) {
  if (patterns.empty()) {
    current_pattern = pattern;
    current_pattern_index = 0;
  } 
  patterns.push_back(pattern);
}

void Pattern::step() {
  if (current_pattern_index >= patterns.size()) {
    return;  
  }
  uint32_t start_time = millis();
  if (current_index >= current_pattern.size()) {
    current_index = 0;
    current_pattern = patterns[++current_pattern_index];
  }
  ArmAngle angle = current_pattern[current_index];
  if (current_index == 0) {
    stepper_motor.moveToStart(angle.stepper_angle);
    servo_motor.moveToStart(angle.servo_angle);
  }
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
