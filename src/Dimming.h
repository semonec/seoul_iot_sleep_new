#include "mbed.h"
#include "WS2812.h"
#include "PixelArray.h"

#define LED_COLOR_R 0xff0000
#define LED_COLOR_G 0x00ff00
#define LED_COLOR_B 0x0000ff

#define MAX_BRIGHGNESS 0.3

void dimmingOn();
void dimmingOff();

enum EventList {
  ButtonOff,
  ButtonOn,
  Crying,
  NoCrying
};

class Dimming {
public:
  Dimming(PinName pin) {
    // See the program page for information on the timing numbers
    // The given numbers are for the K64F
    m_ws = new WS2812(pin, m_led_nums, 0, 5, 5, 0);
    m_ws->useII(WS2812::OFF); // use per-pixel intensity scaling
    
    m_px = new PixelArray(m_led_nums);
    m_dimming_level = 0.05; // pre-defined dimming level value
    m_dimming_step_time = 0.05; // Each pre-defined dimming level's staying time.
    m_is_dim_on = false;
  }
  ~Dimming() {
    delete m_ws;
    delete m_px;
  }

  // level should be 0.0 to 1.0
  void set_px(double level);
  void set_light();
  void on();
  void off();
  void set_led_numbers(int num);
  void set_dimming_level(double level);
  void set_dimming_step_time(double sec);
  bool is_light_on();
private:
  WS2812* m_ws;
  PixelArray* m_px;
  int m_led_nums = 12;
  double m_dimming_level;
  double m_dimming_step_time;
  bool m_is_dim_on;
};