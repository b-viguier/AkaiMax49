#include "WheelManager.h"
#include "Pins.h"

namespace {

  void defaultCallback(byte) {
    // No-Op
  }

  inline byte readByte(byte id) {
    // TODO: improve accuracy…
    return map(analogRead(id), 0b10111, 0b100111, 0, 127);
  }
}

void WheelManager::setup() {
  pinMode(PIN_PITCH_11, INPUT);
  _lastValues[ID::PITCH] = readByte(PIN_PITCH_11);
  _callbacks[ID::PITCH] = defaultCallback;

  pinMode(PIN_MOD_12, INPUT);
  _lastValues[ID::MOD] = readByte(PIN_MOD_12);
  _callbacks[ID::MOD] = defaultCallback;
}

WheelManager& WheelManager::setCallback(ID id, Callback callback) {
  _callbacks[id] = callback;
    return *this;
}

void WheelManager::update() {

  const byte raw[]= {
      readByte(PIN_PITCH_11),
      readByte(PIN_MOD_12)
  };

  for(ID id = ID::PITCH; id <= ID::MOD; id = (ID)(id+1)) {
    if(raw[id] != _lastValues[id]) {
      _callbacks[id](_lastValues[id] = raw[id]);
    }
  }
}