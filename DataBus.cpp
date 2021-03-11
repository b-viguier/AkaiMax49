#include "DataBus.h"
#include "Pins.h"

void DataBus::setup() {
  pinMode(PIN_ADDR_10, OUTPUT);
  pinMode(PIN_ADDR_11, OUTPUT);
  pinMode(PIN_ADDR_12, OUTPUT);
  pinMode(PIN_ADDR_13, OUTPUT);
  pinMode(PIN_ADDR_14, OUTPUT);
  pinMode(PIN_ADDR_15, OUTPUT);
}

void DataBus::setMode(DataBus::Mode mode) {
  if(_mode == mode) {
    return;
  }

  pinMode(PIN_DATA_2, mode);
  pinMode(PIN_DATA_3, mode);
  pinMode(PIN_DATA_4, mode);
  pinMode(PIN_DATA_5, mode);
  pinMode(PIN_DATA_6, mode);
  pinMode(PIN_DATA_7, mode);
  pinMode(PIN_DATA_8, mode);
  pinMode(PIN_DATA_9, mode);
  delay(1);

  _mode = mode;
}

void DataBus::write(byte data) const {
  digitalWrite(PIN_DATA_2, data & 0x01);
  digitalWrite(PIN_DATA_3, data & 0x02);
  digitalWrite(PIN_DATA_4, data & 0x04);
  digitalWrite(PIN_DATA_5, data & 0x08);
  digitalWrite(PIN_DATA_6, data & 0x10);
  digitalWrite(PIN_DATA_7, data & 0x20);
  digitalWrite(PIN_DATA_8, data & 0x40);
  digitalWrite(PIN_DATA_9, data & 0x80);
}

byte DataBus::read() const {
  return (digitalRead(PIN_DATA_2))
      | (digitalRead(PIN_DATA_3) << 1)
      | (digitalRead(PIN_DATA_4) << 2)
      | (digitalRead(PIN_DATA_5) << 3)
      | (digitalRead(PIN_DATA_6) << 4)
      | (digitalRead(PIN_DATA_7) << 5)
      | (digitalRead(PIN_DATA_8) << 6)
      | (digitalRead(PIN_DATA_9) << 7)
      ;
}

void DataBus::setAddress(byte addr) const {
  digitalWrite(PIN_ADDR_10, addr & 0x01);
  digitalWrite(PIN_ADDR_11, addr & 0x02);
  digitalWrite(PIN_ADDR_12, addr & 0x04);
  digitalWrite(PIN_ADDR_13, addr & 0x08);
  digitalWrite(PIN_ADDR_14, addr & 0x10);
  digitalWrite(PIN_ADDR_15, addr & 0x20);
}