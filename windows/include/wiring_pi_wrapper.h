#ifndef WIRING_PI_WRAPPER_H
#define WIRING_PI_WRAPPER_H

#define PWM_OUTPUT 0
#define WIRING_PI_OUTPUT 1
#define WIRING_PI_INPUT 2

#define HIGH 1
#define LOW 0

#define PWM_MODE_MS 0
#define PWM_MODE_BAL 1

#define WIRING_PI_DEBUG true

void wiringPiSetup();

void pwmSetMode(int mode);

void pwmSetClock(int clk);

void pwmSetRange(int range);

void pwmWrite(int pin, int value);

void pinMode(int pin, int mode);

void delay(int millis);

void digitalWrite(int pin, int value);

int digitalRead(int pin);

int millis();

#endif
