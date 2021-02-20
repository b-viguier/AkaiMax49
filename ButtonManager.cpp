#include <Arduino.h>

#include "ButtonManager.h"
#include "Pins.h"


namespace {


  enum ButtonBlockAddr {
    U524 = 0b00001000,
    U221 = 0b00001001,
    U220 = 0b00001010,
    U223 = 0b00001011,
    U222 = 0b00001100,
    U272 = 0b00001101,
  };


  void onButtonChangedDefault(bool new_state) {
    if (new_state) {
      Serial.println("Button pressed");
    } else {
      Serial.println("Button released");
    }
  }


  byte triggerButton(byte state, Pin pin, byte mask, ButtonManager::Callback callback) {
    bool pinValue;
    if ((pinValue = digitalRead(pin)) != (bool) (state & mask)) {
      callback(!pinValue);
      return mask;
    }

    return 0;
  }


  void triggerBlockButtons(ButtonBlockAddr blockAddr, byte &state, ButtonManager::Callback callback1, ButtonManager::Callback callback2,
                           ButtonManager::Callback callback3, ButtonManager::Callback callback4, ButtonManager::Callback callback5,
                           ButtonManager::Callback callback6, ButtonManager::Callback callback7, ButtonManager::Callback callback8
  ) {

    digitalWrite(PIN_ADDR_10, blockAddr & 0b00000001);
    digitalWrite(PIN_ADDR_11, blockAddr & 0b00000010);
    digitalWrite(PIN_ADDR_12, blockAddr & 0b00000100);
    digitalWrite(PIN_ADDR_13, blockAddr & 0b00001000);
    digitalWrite(PIN_ADDR_14, blockAddr & 0b00010000);
    digitalWrite(PIN_ADDR_15, blockAddr & 0b00100000);

    digitalWrite(PIN_CLOCK_BTN_17, LOW);
    delay(1);

    state = state ^ (
        triggerButton(state, PIN_DATA_1, 0b00000001, callback1) |
        triggerButton(state, PIN_DATA_2, 0b00000010, callback2) |
        triggerButton(state, PIN_DATA_3, 0b00000100, callback3) |
        triggerButton(state, PIN_DATA_4, 0b00001000, callback4) |
        triggerButton(state, PIN_DATA_5, 0b00010000, callback5) |
        triggerButton(state, PIN_DATA_6, 0b00100000, callback6) |
        triggerButton(state, PIN_DATA_7, 0b01000000, callback7) |
        triggerButton(state, PIN_DATA_8, 0b10000000, callback8)
    );
    digitalWrite(PIN_CLOCK_BTN_17, HIGH);
  }

}

ButtonManager::ButtonManager()
    : _states{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    _callbacks{} {
// Default callbacks
  for(unsigned int i=0; i<6*8; ++i) {
    _callbacks[i] = onButtonChangedDefault;
  }
}

void ButtonManager::setup() {
}

void ButtonManager::update() {
  for (byte pin = PIN_DATA_1; pin <= PIN_DATA_8; ++pin) {
    pinMode(pin, INPUT);
  }

  triggerBlockButtons(U524, _states[0], _callbacks[TIME_4], _callbacks[TIME_4T],
                                   _callbacks[TIME_8], _callbacks[TIME_8T],
                                   _callbacks[TIME_16], _callbacks[TIME_16T],
                                   _callbacks[TIME_32], _callbacks[TIME_32T]
  );

  triggerBlockButtons(U221, _states[1], _callbacks[BANK_1_8],
                                   _callbacks[BANK_9_16], _callbacks[BANK_17_24],
                                   _callbacks[BANK_25_32], _callbacks[MODE_SEQ_PITCH],
                                   _callbacks[MODE_SEQ_CC], _callbacks[MODE_MIDI_CC],
                                   _callbacks[TIME_DIV]
  );
  triggerBlockButtons(U220, _states[2], _callbacks[UP], _callbacks[DOWN],
                                   _callbacks[LEFT], _callbacks[RIGHT],
                                   _callbacks[EDIT_STORE], _callbacks[PROGRAM_STD],
                                   _callbacks[GLOBAL_HUI], _callbacks[PROG_CHANGE]
  );
  triggerBlockButtons(U223, _states[3], _callbacks[PREV], _callbacks[STOP],
                                   _callbacks[PLAY], _callbacks[RECORD], _callbacks[NEXT], _callbacks[SHIFT],
                                   _callbacks[TEMPO], _callbacks[ENTER]
  );
  triggerBlockButtons(U222, _states[4], _callbacks[OCT_DOWN], _callbacks[OCT_UP],
                                   _callbacks[PLAY], _callbacks[RECORD], _callbacks[NEXT], _callbacks[SHIFT],
                                   _callbacks[ARP_MODE], _callbacks[SEQ_SELECT]
  );
  triggerBlockButtons(U272, _states[5], _callbacks[ON_OFF], _callbacks[LATCH],
                                   _callbacks[NOTE_REPEAT], _callbacks[PAD_BANK_A],
                                   _callbacks[PAD_BANK_B], _callbacks[PAD_BANK_C], _callbacks[PAD_BANK_D],
                                   _callbacks[FULL_LEVEL]
  );

  /**
   * /!| Play, Record, next, Shift
   */
}

ButtonManager& ButtonManager::setCallback(ID id, Callback callback)
{
  _callbacks[id] = callback;
  return *this;
}
