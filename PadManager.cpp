#include "PadManager.h"
#include "Pins.h"

namespace {

  void defaultCallback(byte id, byte value) {
    Serial.print("Pad [");
    Serial.print(id);
    Serial.print("] ");
    Serial.println(value);
  }

  void triggerRow(byte rowPin, byte rowId, byte previous[PadManager::NB_PADS], PadManager::Callback callback) {
    digitalWrite(rowPin, LOW);
    delay(1);
    const uint32_t colPin[PadManager::NB_COLS] = {
        PIN_PAD_COL_26,
        PIN_PAD_COL_27,
        PIN_PAD_COL_28,
        PIN_PAD_COL_29,
    };
    for(short colId = 0; colId < PadManager::NB_COLS; ++colId) {
      const auto analogValue = analogRead(colPin[colId]);
      // TODO: accuracy
      const byte value = map(constrain(analogValue, 2, 50), 2, 50, 0, 127);
      const short padId = rowId * PadManager::NB_COLS + colId;
      if(value == previous[padId]) {
        continue;
      }
      previous[padId] = value;
      callback(padId, value);
    }

    digitalWrite(rowPin, HIGH);
  }
}

PadManager::PadManager()
    : _callback(&defaultCallback)
    , _values{0}
{

}

void PadManager::setup()
{
  pinMode(PIN_PAD_ROW_22, OUTPUT); digitalWrite(PIN_PAD_ROW_22, HIGH);
  pinMode(PIN_PAD_ROW_23, OUTPUT); digitalWrite(PIN_PAD_ROW_23, HIGH);
  pinMode(PIN_PAD_ROW_24, OUTPUT); digitalWrite(PIN_PAD_ROW_24, HIGH);
  pinMode(PIN_PAD_COL_26, INPUT);
  pinMode(PIN_PAD_COL_27, INPUT);
  pinMode(PIN_PAD_COL_28, INPUT);
  pinMode(PIN_PAD_COL_29, INPUT);
}

void PadManager::update()
{
  triggerRow(PIN_PAD_ROW_24, 0, _values, _callback);
  triggerRow(PIN_PAD_ROW_23, 1, _values, _callback);
  triggerRow(PIN_PAD_ROW_22, 2, _values, _callback);
}

PadManager& PadManager::setCallback(Callback callback)
{
  _callback = callback;

  return *this;
}
