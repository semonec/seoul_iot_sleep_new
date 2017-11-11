#include "Dimming.h"

Dimming dim(D9);

void dimmingOn() {
  dim.on();    
}

void dimmingOff() {
  dim.off();
}

// level should be 0.0 to 1.0
void Dimming::set_px(double level) {
  int adjusted_level = (int)(255 * level);
  int leveled_color_r = adjusted_level * 65536;
  int leveled_color_g = adjusted_level * 256;
  int leveled_color_b = adjusted_level * 1;
  int color = leveled_color_r + leveled_color_g + leveled_color_b;  
  for (int i = 0; i < m_led_nums; i++) {
    m_px->Set(i, color);
  } 
}

void Dimming::set_light() {
  for (int z=m_led_nums; z >= 0 ; z--) {
    m_ws->write_offsets(m_px->getBuf(),z,z,z);
  }
}

void Dimming::on() {
  if (m_is_dim_on)
    return;
  m_is_dim_on = true;
  for(double i = 0.0; i <= MAX_BRIGHGNESS; i+= m_dimming_level) {
    set_px(i);
    set_light();
    wait(m_dimming_step_time);
  }
  set_px(MAX_BRIGHGNESS);
  set_light();
}

void Dimming::off() {
  if (!m_is_dim_on)
    return;
  m_is_dim_on = false;      
  for(double i = MAX_BRIGHGNESS; i >= 0.0; i-= m_dimming_level) {
    set_px(i);
    set_light();
    wait(m_dimming_step_time);
  }
  set_px(0);
  set_light();
}

void Dimming::set_led_numbers(int num) {
  m_led_nums = num;
}
void Dimming::set_dimming_level(double level) {
  m_dimming_level = level;
}
void Dimming::set_dimming_step_time(double sec) {
  m_dimming_step_time = sec;
}

bool Dimming::is_light_on() {
  return m_is_dim_on;
}