#include "pattern_manager.h"

#include "servo_motor.h"
#include "sisyphus_util.h"
#include "stepper_motor.h"
#include "model.pb.h"
#include "wiringPi.h"
#include <cstdint>

PatternManager::PatternManager() {
  current_index = 0;
  current_pattern_index = 0;
  wiringPiSetup();
  stepper_motor.setup();
  servo_motor.setup();
}

void PatternManager::queuePattern(std::vector<sisyphus::ArmAngle> pattern) {
  lock.lock();
  if (patterns.empty()) {
    patterns.push_back(pattern);
    current_pattern = pattern;
    current_pattern_index = 0;
  } else {
    patterns.insert(patterns.begin() + current_pattern_index, pattern);
    // Can't overflow
    current_pattern_index++;
    current_pattern = patterns[current_pattern_index];
  }
  lock.unlock();
}

void PatternManager::step() {
  uint32_t last_step_time = millis();
  lock.lock();
  if (current_index >= current_pattern.size()) {
    current_index = 0;
    if (++current_pattern_index >= patterns.size()) {
      current_pattern_index = 0;
    }
    current_pattern = patterns[current_pattern_index];
  }
  sisyphus::ArmAngle angle = current_pattern[current_index];
  if (current_index == 0) {
    stepper_motor.moveToStart(angle.stepper_angle());
    servo_motor.moveToStart(angle.servo_angle());
  }
  current_index++;
  lock.unlock();
  bool keep_going =
      servo_motor.step(angle.servo_angle())
          && stepper_motor.step(angle.stepper_angle());
  while (!keep_going) {
    keep_going =
        servo_motor.step(angle.servo_angle())
            && stepper_motor.step(angle.stepper_angle());
  }
  uint32_t diff = millis() - last_step_time;
  if (diff < 100) {
    delay(100 - diff);
  }
}
