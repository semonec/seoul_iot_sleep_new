#include "mbed.h"
#include "DHT.h"
#include "simplem2mclient.h"

DHT dht(D5, DHT11);
static M2MResource* temp_res; // Resource to temp
static M2MResource* humi_res; // Resource to humidity

void read_temp() {
  int error = 0;
  float c = 0.0f;
  error = dht.readData();
  if (0 == error) {
      c = dht.ReadTemperature(CELCIUS);
      printf("Celcius is %4.2f\n", c);
      char val[13];
      sprintf(val, "%.2f", c);
      temp_res->set_value((uint8_t*)val, strlen(val));
  } else {
      printf("Error read celcius : %d\n", error);
  }
}

void read_humi() {
  int error = 0;
  float h = 0.0f;
  error = dht.readData();
  if (0 == error) {
      h   = dht.ReadHumidity();
      printf("Humidity is %4.2f\n", h);
      char val[13];
      sprintf(val, "%.2f", h);
      humi_res->set_value((uint8_t*)val, strlen(val));
  } else {
      printf("Error read humi : %d\n", error);
  }
}

void read_temp_humi() {
    int error = 0;
    float h = 0.0f, c = 0.0f;

    error = dht.readData();
    if (0 == error) {
        // read and update humidity
        h = dht.ReadHumidity();
        char val_humi[13];
        sprintf(val_humi, "%.2f", h);
        humi_res->set_value((uint8_t*)val_humi, strlen(val_humi));

        // read and update temperature
        c = dht.ReadTemperature(CELCIUS);
        printf("Humidity is %4.2f\t, Celcius is %4.2f\n", h, c);
        char val_temp[13];
        sprintf(val_temp, "%.2f", c);
        temp_res->set_value((uint8_t*)val_temp, strlen(val_temp));
    } else {
    }
}

void register_temp_humi_cloud_resource(SimpleM2MClient* mbedClient) {
    temp_res = mbedClient->add_cloud_resource(3303, 0, 5700, "temp_resource", M2MResourceInstance::FLOAT,M2MBase::GET_ALLOWED, 0, false, (void*)read_humi);
    humi_res = mbedClient->add_cloud_resource(3304, 0, 5700, "humi_resource", M2MResourceInstance::FLOAT,M2MBase::GET_ALLOWED, 0, false, (void*)read_temp);
}