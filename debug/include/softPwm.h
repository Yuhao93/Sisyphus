#ifndef SOFT_PWM_H
#define SOFT_PWM_H

void softPwmCreate(int pin, int initialValue, int range);
void softPwmWrite(int pin, int value);

#endif
