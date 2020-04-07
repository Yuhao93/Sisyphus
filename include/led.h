#ifndef LED_H
#define LED_H

class Led {
public:
  Led();

  /**
   * An intensity value between 0 and 1. 0 is off, 1 is fully bright.
   */
  void setIntensity(double intensity);
  double getIntensity();

private:
  double intensity_;
};

#endif
