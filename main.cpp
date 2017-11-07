/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if defined(TARGET_K64F) 
#define BUTTON SW2
#else 
#define BUTTON SW0
#endif 
#include "simplem2mclient.h"
//#include "base.h"
#include "Dimming.h"

EventQueue queue;
Thread eventThread;

// for dimming leds
// D2 = button's pin, D4 = neo pixel's pin
InterruptIn button(D2);
DHT sensor(D4, DHT11);

// Main Event handler
// All event should be handled in here
void dispatcher(int event_type) {
    printf("Event Dispatcher: eventType: %d\n", event_type);
  
    switch(event_type) {
      case ButtonOn:
        dim.on();  
      break;
      case ButtonOff:
        dim.off();
      break;
    }
}

int main()
{
    printf("Simple M2M client example runs\n");
    // SimpleClient is used for registering and unregistering resources to a server.
    SimpleM2MClient mbedClient;

    if (!mbedClient.init()) {
        printf("Initialization failed, exiting application!\n");
        return 1;
    }
    // Event queue will run from sub thread
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    // controlling button event
    // fail event menas 1 -> 0 chnages, rise means 0 -> 1 changes
    button.fall(queue.event(dispatcher, ButtonOff));
    button.rise(queue.event(dispatcher, ButtonOn));

    // TODO: blocked should be re-implemented
    // potentiometer_res = mbedClient.add_cloud_resource(3200, 0, 5501, "potentiometer_resource", M2MResourceInstance::FLOAT, M2MBase::GET_ALLOWED, 0, true, NULL);
    // message_res = mbedClient.add_cloud_resource(3201, 0, 5853, "message_resource", M2MResourceInstance::STRING, M2MBase::GET_PUT_ALLOWED, "Hello world!", false, (void*)message_updated_callback);
    // display_res = mbedClient.add_cloud_resource(3201, 0, 5850, "display_resource", M2MResourceInstance::STRING,M2MBase::POST_ALLOWED, "", false, (void*)display_message_callback);
    // led_res = mbedClient.add_cloud_resource(3202, 0, 5853, "led_resource", M2MResourceInstance::INTEGER,M2MBase::GET_PUT_ALLOWED, 0, false, (void*)set_blink_led);

    mbedClient.start_client();
    // queue.call_every(100, read_potentiometer);
    while(mbedClient.is_register_called()){
        wait_ms(100);
        queue.dispatch(0);
    }
}
