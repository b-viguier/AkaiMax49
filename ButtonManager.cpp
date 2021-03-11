#include <Arduino.h>

#include "ButtonManager.h"
#include "Pins.h"
#include "DataBus.h"


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


  void triggerBlockButtons(const DataBus& dataBus, ButtonBlockAddr blockAddr, byte &state, ButtonManager::Callback callback1, ButtonManager::Callback callback2,
                           ButtonManager::Callback callback3, ButtonManager::Callback callback4, ButtonManager::Callback callback5,
                           ButtonManager::Callback callback6, ButtonManager::Callback callback7, ButtonManager::Callback callback8
  ) {

    dataBus.setAddress(blockAddr);

    digitalWrite(PIN_CLOCK_BTN_17, LOW);
    delay(1);
    const auto new_state = dataBus.read();
    digitalWrite(PIN_CLOCK_BTN_17, HIGH);

    const byte diff = new_state ^ state;
    if(diff) {
      const ButtonManager::Callback callbacks[8] = {callback1, callback2, callback3, callback4, callback5, callback6, callback7, callback8};
      for(byte i=0, mask=1; i<8; ++i, mask = mask<<1) {
        if(diff & mask) {
          // Remember that 0 means pushed
          callbacks[i](mask & state);
        }
      }
      state = new_state;
    }

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
  digitalWrite(PIN_CLOCK_BTN_17, HIGH);
  pinMode(PIN_CLOCK_BTN_17, OUTPUT);
}

void ButtonManager::update(DataBus& dataBus) {
  dataBus.setMode(DataBus::BUS_IN);

  triggerBlockButtons(dataBus, U524, _states[0], _callbacks[TIME_4], _callbacks[TIME_4T],
                                   _callbacks[TIME_8], _callbacks[TIME_8T],
                                   _callbacks[TIME_16], _callbacks[TIME_16T],
                                   _callbacks[TIME_32], _callbacks[TIME_32T]
  );

  triggerBlockButtons(dataBus, U221, _states[1], _callbacks[BANK_1_8],
                                   _callbacks[BANK_9_16], _callbacks[BANK_17_24],
                                   _callbacks[BANK_25_32], _callbacks[MODE_SEQ_PITCH],
                                   _callbacks[MODE_SEQ_CC], _callbacks[MODE_MIDI_CC],
                                   _callbacks[TIME_DIV]
  );
  triggerBlockButtons(dataBus, U220, _states[2], _callbacks[UP], _callbacks[DOWN],
                                   _callbacks[LEFT], _callbacks[RIGHT],
                                   _callbacks[EDIT_STORE], _callbacks[PROGRAM_STD],
                                   _callbacks[GLOBAL_HUI], _callbacks[PROG_CHANGE]
  );
  triggerBlockButtons(dataBus, U223, _states[3], _callbacks[PREV], _callbacks[STOP],
                                   _callbacks[PLAY], _callbacks[RECORD], _callbacks[NEXT], _callbacks[SHIFT],
                                   _callbacks[TEMPO], _callbacks[ENTER]
  );
  triggerBlockButtons(dataBus, U222, _states[4], _callbacks[OCT_DOWN], _callbacks[OCT_UP],
                                   _callbacks[PLAY], _callbacks[RECORD], _callbacks[NEXT], _callbacks[SHIFT],
                                   _callbacks[ARP_MODE], _callbacks[SEQ_SELECT]
  );
  triggerBlockButtons(dataBus, U272, _states[5], _callbacks[ON_OFF], _callbacks[LATCH],
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
