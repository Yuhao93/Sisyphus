#ifndef STRUCTS_H
#define STRUCTS_H

struct PolarCoordinate {
  float r;
  float a;
};

struct ArmAngle {
  float stepper_angle;
  float servo_angle;
};

struct CartesianCoordinate {
  float x;
  float y;
};

#endif
