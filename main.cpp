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

#include "simplem2mclient.h"
//#include "base.h"
#include "Dimming.h"
#include "temp_humi.h"
#include "mbed.h"
#include "preassure.h"
#include "sound.h"

EventQueue queue;
Thread eventThread;

// for dimming leds
// D2 = button's pin, D9 = neo pixel's pin
InterruptIn d2(A0);
InterruptIn d3(D3);

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
      case Crying:
        printf("crying\n");
        break;
    }
}

// void update_dimming() {
//     int value = button.read();
//     printf("button value:%d\n", value);
// }

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
    d3.mode(PullUp);
    ////////////////////////////////////////////////////////////////////////////
    // NeoPixel
    // controlling button event
    // fall event menas 1 -> 0 chnages, rise means 0 -> 1 changes
    // button.mode(PullUp);
    d2.fall(queue.event(dispatcher, ButtonOn));
    d2.rise(queue.event(dispatcher, ButtonOff));
    d3.rise(queue.event(dispatcher, Crying));
    ////////////////////////////////////////////////////////////////////////////
    // Registering sensor into mbed cloud
    // blocked shield sensor registering 
    //register_shield_sensors(&mbedClient);
    register_temp_humi_cloud_resource(&mbedClient);
    register_pressure_cloud_resource(&mbedClient);
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
    queue.call_every(5000, read_temp_humi);
    queue.call_every(500, read_sound);
    queue.call_every(500, read_pressure);

    //queue.call_every(500, update_dimming);
    /*
     * update sensor's readed data to mbed cloud
     */
    // HERE
    //queue.call_every(500, read_ultra);
    while(mbedClient.is_register_called()){
        wait_ms(2000);
        //queue.dispatch(0);
    }
}
