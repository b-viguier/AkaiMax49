#ifndef AKAIMAX49_PINS_H
#define AKAIMAX49_PINS_H

#include <Arduino.h>

enum Pin {
  PIN_DATA_2 = 53,
  PIN_DATA_3 = 52,
  PIN_DATA_4 = 51,
  PIN_DATA_5 = 50,
  PIN_DATA_6 = 49,
  PIN_DATA_7 = 48,
  PIN_DATA_8 = 47,
  PIN_DATA_9 = 46,

  PIN_ADDR_10 = 45,
  PIN_ADDR_11 = 44,
  PIN_ADDR_12 = 43,
  PIN_ADDR_13 = 42,
  PIN_ADDR_14 = 41,
  PIN_ADDR_15 = 40,

  PIN_CLOCK_LED_16 = 39,
  PIN_CLOCK_BTN_17 = 38,

  PIN_ENABLE_19 = 37,

  PIN_ROT_LEFT_20 = 36,
  PIN_ROT_RIGHT_21 = 35,

  PIN_LCD_ENABLE_22 = 34,
  PIN_LCD_RW_23 = 33,
  PIN_LCD_RS_24 = 32,
  PIN_LCD_V0_25 = DAC1,

  PIN_PITCH_11 = A0,
  PIN_MOD_12 = A1,

  PIN_SLIDER_ADDR_13 = 30,
  PIN_SLIDER_ADDR_14 = 31,
  PIN_SLIDER_INPUT_17 = A2,
  PIN_SLIDER_INPUT_18 = A3,

  PIN_PAD_ROW_22 = 29,
  PIN_PAD_ROW_23 = 28,
  PIN_PAD_ROW_24 = 27,
  PIN_PAD_COL_26 = A4,
  PIN_PAD_COL_27 = A5,
  PIN_PAD_COL_28 = A6,
  PIN_PAD_COL_29 = A7,
};

#endif //AKAIMAX49_PINS_H
