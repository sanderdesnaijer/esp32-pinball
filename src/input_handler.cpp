#include "input_handler.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <driver/touch_pad.h>
#endif

InputHandler::InputHandler() : leftFlipperActive(false), rightFlipperActive(false) {}

void InputHandler::initialize() {
#ifdef ARDUINO
  touch_pad_init();
  touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_0V);
  touch_pad_config(TOUCH_PAD_NUM4, TOUCH_THRESHOLD);
  touch_pad_config(TOUCH_PAD_NUM5, TOUCH_THRESHOLD);
  touch_pad_config(TOUCH_PAD_NUM6, TOUCH_THRESHOLD);
  touch_pad_config(TOUCH_PAD_NUM7, TOUCH_THRESHOLD);
  touch_pad_config(TOUCH_PAD_NUM8, TOUCH_THRESHOLD);
  touch_pad_config(TOUCH_PAD_NUM9, TOUCH_THRESHOLD);
  touch_pad_filter_start(10);
#endif
}

void InputHandler::update() {
  readTouchPads();
}

void InputHandler::readTouchPads() {
#ifdef ARDUINO
  // Touch pad layout (viewed from front of machine):
  //   Right side: T4 (GPIO4), T5, T6, T7
  //   Left side:  T8 (GPIO33), T9 (GPIO32)
  uint16_t t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;
  touch_pad_read_raw_data(TOUCH_PAD_NUM4, &t4);
  touch_pad_read_raw_data(TOUCH_PAD_NUM5, &t5);
  touch_pad_read_raw_data(TOUCH_PAD_NUM6, &t6);
  touch_pad_read_raw_data(TOUCH_PAD_NUM7, &t7);
  touch_pad_read_raw_data(TOUCH_PAD_NUM8, &t8);
  touch_pad_read_raw_data(TOUCH_PAD_NUM9, &t9);

  leftFlipperActive  = (t8 < TOUCH_THRESHOLD) || (t9 < TOUCH_THRESHOLD);
  rightFlipperActive = (t4 < TOUCH_THRESHOLD) || (t5 < TOUCH_THRESHOLD)
                    || (t6 < TOUCH_THRESHOLD) || (t7 < TOUCH_THRESHOLD);

  Serial.printf("R: T4=%4d T5=%4d T6=%4d T7=%4d | L: T8=%4d T9=%4d | thr=%d\n",
                t4, t5, t6, t7, t8, t9, TOUCH_THRESHOLD);
#else
  leftFlipperActive = false;
  rightFlipperActive = false;
#endif
}
