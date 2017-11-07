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
#include "temp_humi.h"

EventQueue queue;
Thread eventThread;

// for dimming leds
// D2 = button's pin, D9 = neo pixel's pin
InterruptIn button(D2);

// Dispatcher
// directly updated value will be handled here (by user interaction)
void dispatcher(int event_type) {
    printf("Event Dispatcher: eventType: %d\n", event_type);
    switch(event_type) {
      case ButtonOn:
        dimmingOn();
      break;
      case ButtonOff:
        dimmingOff();
      break;
    }
}

int main()
{
    // SimpleClient is used for registering and unregistering resources to a server.
    SimpleM2MClient mbedClient;
    if (!mbedClient.init()) {
        printf("Initialization failed, exiting application!\n");
        return 1;
    }
    // Event queue will run from sub thread
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    ////////////////////////////////////////////////////////////////////////////
    // NeoPixel
    // controlling button event
    // fail event menas 1 -> 0 chnages, rise means 0 -> 1 changes
    button.fall(queue.event(dispatcher, ButtonOff));
    button.rise(queue.event(dispatcher, ButtonOn));


    ////////////////////////////////////////////////////////////////////////////
    // Registering sensor into mbed cloud
    // blocked shield sensor registering 
    //register_shield_sensors(&mbedClient);
    register_temp_humi_cloud_resource(&mbedClient);
    /*
     * register sensor to mbed cloud
     */
    // HERE





    mbedClient.start_client();
    ////////////////////////////////////////////////////////////////////////////
    // Update sensor's readed data into mbed cloud
    // shield potentionmeter reading 
    // queue.call_every(100, read_potentiometer);    
    // temp & humi data update to mbed cloud
    queue.call_every(500, read_temp_humi);
    /*
     * update sensor's readed data to mbed cloud
     */
    // HERE



    while(mbedClient.is_register_called()){
        wait_ms(2000);
        queue.dispatch(0);
    }
}
