
#include "simplem2mclient.h"
#include "mbed.h"
#include "C12832.h"

C12832 lcd(D11, D13, D6, D7, D10);
AnalogIn pot1(A0);
EventQueue queue;
DigitalOut led(D9, 1);

// DECLARE RESOURCES HERE
static M2MResource* potentiometer_res; // Resource for potentiometer value
static M2MResource* message_res; // Resource to hold a string message
static M2MResource* display_res; // Resource for displaying message on LCD screen
static M2MResource* led_res; // Resource to blink LED

void lcd_print(const char* message) {
  lcd.cls();
  lcd.locate(0,3);
  lcd.printf(message);
}

void blink_led() {
  led = !led;
}

void set_blink_led(int *) {
  static int blink_id = NULL;
  // GET VALUE OF LED RESOURCE
  int blink = led_res->get_value_int();
  if (blink == 1 && blink_id == NULL) {
    blink_id = queue.call_every(500, blink_led);
  }
  else if (blink == 0) {
    queue.cancel(blink_id);
    blink_id = NULL;
    led = 1;
  }
}

// MESSAGE RESOURCE PUT HANDLER
void message_updated_callback(const char *)
{
  printf("PUT received for message resource, new value: %s\n", message_res->get_value_string().c_str());
}

// DISPLAY RESOURCE POST HANDLER
void display_message_callback(void *) {
  // Get the value of the message resource
  String pattern_string = message_res->get_value_string();
  // Display it on the LCD screen
  lcd_print(pattern_string.c_str());
}

void read_potentiometer() {
  static float potentiometer_val = 0;
  if ((float)pot1 != potentiometer_val) {
    potentiometer_val = (float)pot1;
    char val[13];
    sprintf(val, "%.2f", potentiometer_val);
    // UPDATE RESOURCE VALUE
    potentiometer_res->set_value((uint8_t*)val, strlen(val));
  }
}
