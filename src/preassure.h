#include "mbed.h"
#include "simplem2mclient.h"

#define PREASSURE_DETECT 0.3
static M2MResource* pressure_res; // Resource to temp

AnalogIn preassure(A0);

bool is_pressed() {
  float value = preassure.read();
  //printf("readed: %f \n", value);
  return value >= PREASSURE_DETECT;
}

void read_pressure() {
  bool pressed = is_pressed();
  pressure_res->set_value(pressed? 1: 0);
}

void register_pressure_cloud_resource(SimpleM2MClient* mbedClient) {
    pressure_res = mbedClient->add_cloud_resource(3302, 0, 5500, "pressure_status_resource", M2MResourceInstance::BOOLEAN,M2MBase::GET_ALLOWED, 0, false, (void*)read_pressure);
}