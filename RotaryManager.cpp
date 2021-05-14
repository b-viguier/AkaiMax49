#include "RotaryManager.h"
#include "Pins.h"

namespace {

  void defaultCallback(int count) {
    Serial.print("Rotary: ");
    Serial.println(count);
  }

  enum Val {
    INIT = 0b10,

    INC_CLOCKWISE = 1,
    INC_COUNTER_CLOCKWISE = 5,
    INC_UNKNOWN = 0,
    INC_TOO_BIG = 2,

    NB_STEPS = 4,
  };

  volatile struct RotaryState {
    byte _state;
    byte _inc;
    int _count;

    RotaryState()
    : _state(Val::INIT)
    , _inc(Val::INC_UNKNOWN)
    , _count(0)
    {
      // Assume that rotary encoder is in 0 state
    }

  } rotaryState;

  void rotaryInterrupt() {
    // TODO: 1 interrupt per pin?

    const byte left_state = digitalRead(PIN_ROT_LEFT_20);
    // Ensure that state behave like a counter (mod 4)
    const byte new_state = (left_state << 1) | (left_state ^ digitalRead(PIN_ROT_RIGHT_21));

    if(new_state == rotaryState._state) {
      return;
    }

    const byte new_inc = ((byte)(new_state - rotaryState._state)) % Val::NB_STEPS;

    if(new_inc == Val::INC_TOO_BIG) {
      rotaryState._state = Val::INIT;
      rotaryState._inc = Val::INC_UNKNOWN;
      return;
    }

    if(rotaryState._state == Val::INIT) {
      // New cycle init
      rotaryState._inc = new_inc;
    }

    rotaryState._state = new_state;

    if(new_state == Val::INIT) {
      if(rotaryState._inc == new_inc) {
        if (rotaryState._inc == Val::INC_CLOCKWISE) {
          ++rotaryState._count;
        } else {
          --rotaryState._count;
        }
      }
      rotaryState._inc = Val::INC_UNKNOWN;
    }
  }
}


RotaryManager::RotaryManager()
: _callback(&defaultCallback)
{
}

void RotaryManager::setup() {
  pinMode(PIN_ROT_LEFT_20, INPUT);
  pinMode(PIN_ROT_RIGHT_21, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_ROT_LEFT_20), rotaryInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ROT_RIGHT_21), rotaryInterrupt, CHANGE);

}

void RotaryManager::update() {
  if(rotaryState._count == 0) {
    return;
  }

  _callback(rotaryState._count);
  rotaryState._count = 0;
}

RotaryManager& RotaryManager::setCallback(Callback callback) {
  _callback = callback;

  return *this;
}