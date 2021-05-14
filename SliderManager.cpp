#include "SliderManager.h"
#include "Pins.h"

namespace {

  void defaultCallback(byte id, byte value) {
    Serial.print("Slider [");
    Serial.print(id);
    Serial.print("] ");
    Serial.println(value);
  }

  void triggerCallback(byte id, int value, byte &previous, SliderManager::Callback callback)
  {
    if(value <= 6 ) {
      // No contact
      return;
    }

    const byte fixedValue = map(value, 7, 60, 0, 127);
    if(fixedValue != previous) {
      previous = fixedValue;
      callback(id, fixedValue);
    }
  }
}

SliderManager::SliderManager()
: _callback(&defaultCallback)
, _values{0}
{

}

void SliderManager::setup()
{
  pinMode(PIN_SLIDER_ADDR_13, OUTPUT);
  pinMode(PIN_SLIDER_ADDR_14, OUTPUT);
  pinMode(PIN_SLIDER_INPUT_17, INPUT);
  pinMode(PIN_SLIDER_INPUT_18, INPUT);
}

void SliderManager::update()
{
  for(byte addr1 = 0; addr1 <= 1; ++addr1) {
    digitalWrite(PIN_SLIDER_ADDR_14, addr1);
    for(byte addr2 = 0; addr2 <= 1; ++addr2) {
      digitalWrite(PIN_SLIDER_ADDR_13, addr2);
      delay(1);

      const byte id1 = (addr1 << 1) + addr2;
      triggerCallback(id1, analogRead(PIN_SLIDER_INPUT_17), _values[id1], _callback);
      const byte id2 = 0b100 + id1;
      triggerCallback(id2, analogRead(PIN_SLIDER_INPUT_18), _values[id2], _callback);
    }
  }
}

SliderManager& SliderManager::setCallback(Callback callback)
{
  _callback = callback;

  return *this;
}
