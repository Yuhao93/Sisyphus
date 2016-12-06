#include "wiring_pi_wrapper.h"

#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <windows.h>

namespace wiringPiImpl {
std::unordered_map<int, int> pinModes;
int pwmMode;
int pwmClock;
int pwmRange;
int pwmBaseFrequency = 19200000;
int pwmFrequency;
}

void wiringPiSetup() {
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: wiringPiSetup() called.\n");
  }
}

void pwmSetMode(int mode) {
  wiringPiImpl::pwmMode = mode;
  if (WIRING_PI_DEBUG) {
    const char* mode_str = "";
    if (mode == PWM_MODE_MS) {
      mode_str = "MS";
    } else {
      mode_str = "BAL";
    }
    printf("[WiringPi DEBUG]: Set pwm mode to %s\n", mode_str);
  }
}

void pwmSetClock(int clk) {
  wiringPiImpl::pwmClock = clk;
  wiringPiImpl::pwmFrequency = wiringPiImpl::pwmBaseFrequency / clk;
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: Set pwm clk to %d\n", clk);
  }
}

void pwmSetRange(int range) {
  wiringPiImpl::pwmRange = range;
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: Set pwm range to %d\n", range);
  }
}

void pwmWrite(int pin, int value) {
  float dutyCycle = (float) value / (float) wiringPiImpl::pwmRange;
  float period = 1000.0f * wiringPiImpl::pwmRange / wiringPiImpl::pwmFrequency;
  float live = period * dutyCycle;
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: Pwm written with period %f and uptime %f\n",
        period, live);
  }
}

void pinMode(int pin, int mode) {
  wiringPiImpl::pinModes[pin] = mode;
  if (WIRING_PI_DEBUG) {
    const char* mode_str = "";
    if (mode == WIRING_PI_OUTPUT) {
      mode_str = "OUTPUT";
    } else {
      mode_str = "OUTPUT_PWM";
    }
    printf("[WiringPi DEBUG]: Set pin %d to %s\n", pin, mode_str);
  }
}

void delay(int millis) {
  Sleep(millis);
}

void digitalWrite(int pin, int value) {
  if (WIRING_PI_DEBUG) {
    printf("[WiringPi DEBUG]: Wrote %d to pin %d\n", value, pin);
  }
}

int digitalRead(int pin) {
  return HIGH;
}

int millis() {
  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
  return millis.count();
}
