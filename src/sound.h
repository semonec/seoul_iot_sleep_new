#include "mbed.h"
#include "MAX4466.h"

MAX4466 mic(A1);

float get_sound() {
    mic.volume_indicator();
    float value = mic.sound_level();
    printf("Level is %f\n", value);
    return value;
}

void read_sound() {
  float value = get_sound();
  DigitalOut d3(D3);

  if (value > 0.3) {
    printf("======crying ==> \n");
    d3 = 1;
  } else {
    d3 = 0;
  }
}