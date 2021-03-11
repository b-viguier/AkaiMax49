#include <Arduino.h>

#include "LedManager.h"
#include "Pins.h"
#include "DataBus.h"

static_assert(LedManager::LAST <= 32*8, "Too much buttons declared");

namespace {
  enum LedBlockAddr {
    U520_U521 = 0b00000000,
    U522_523 = 0b00000001,
    U251 = 0b00000010,
    U252 = 0b00000011,
    U267_268 = 0b00000100,
    U269_270 = 0b00000101,
    U271 = 0b00000110,
    U310 = 0b00000111,
    U312 = 0b00001000,
    U316 = 0b00001001,
    U317 = 0b00001010,
    U311 = 0b00001011,
    U313 = 0b00001100,
    U318 = 0b00001101,
    U319 = 0b00001110,
    U314 = 0b00001111,
    U315 = 0b00010000,
    U320 = 0b00010001,
    U321 = 0b00010010,
    U410 = 0b00010011,
    U413 = 0b00010100,
    U416 = 0b00010101,
    U417 = 0b00010110,
    U422 = 0b00010111,
    U411 = 0b00011000,
    U414 = 0b00011001,
    U418 = 0b00011010,
    U419 = 0b00011011,
    U412 = 0b00011100,
    U415 = 0b00011101,
    U420 = 0b00011110,
    U421 = 0b00011111,
  };


  void lightBlock(const DataBus& dataBus, LedBlockAddr addr, byte data) {

    dataBus.setAddress(addr);
    dataBus.write(data);

    delay(1);
    digitalWrite(PIN_CLOCK_LED_16, LOW);
    delay(1);
    digitalWrite(PIN_CLOCK_LED_16, HIGH);
  }

}


LedManager::LedManager()
  : _states {0}
  , _blocksChanged(0xFFFFFFFF)
  {
}


void LedManager::setup(DataBus& dataBus) {
  // Disable LEDs
  digitalWrite(PIN_ENABLE_19, HIGH);
  pinMode(PIN_ENABLE_19, OUTPUT);

  digitalWrite(PIN_CLOCK_LED_16, HIGH);
  pinMode(PIN_CLOCK_LED_16, OUTPUT);
//  delay(1000);

  update(dataBus);
  // Enable LEDs
  digitalWrite(PIN_ENABLE_19, LOW);
}

void LedManager::update(DataBus& dataBus) {

  if(_blocksChanged == 0) {
    return;
  }

  dataBus.setMode(DataBus::BUS_OUT);

  if(_blocksChanged & 0x00000001) lightBlock(dataBus, U310, _states[0]);
  if(_blocksChanged & 0x00000002) lightBlock(dataBus, U312, _states[1]);
  if(_blocksChanged & 0x00000004) lightBlock(dataBus, U316, _states[2]);
  if(_blocksChanged & 0x00000008) lightBlock(dataBus, U317, _states[3]);
  if(_blocksChanged & 0x00000010) lightBlock(dataBus, U311, _states[4]);
  if(_blocksChanged & 0x00000020) lightBlock(dataBus, U313, _states[5]);
  if(_blocksChanged & 0x00000040) lightBlock(dataBus, U318, _states[6]);
  if(_blocksChanged & 0x00000080) lightBlock(dataBus, U319, _states[7]);
  if(_blocksChanged & 0x00000100) lightBlock(dataBus, U314, _states[8]);
  if(_blocksChanged & 0x00000200) lightBlock(dataBus, U315, _states[9]);
  if(_blocksChanged & 0x00000400) lightBlock(dataBus, U320, _states[10]);
  if(_blocksChanged & 0x00000800) lightBlock(dataBus, U321, _states[11]);
  if(_blocksChanged & 0x00001000) lightBlock(dataBus, U410, _states[12]);
  if(_blocksChanged & 0x00002000) lightBlock(dataBus, U413, _states[13]);
  if(_blocksChanged & 0x00004000) lightBlock(dataBus, U416, _states[14]);
  if(_blocksChanged & 0x00008000) lightBlock(dataBus, U417, _states[15]);
  if(_blocksChanged & 0x00010000) lightBlock(dataBus, U422, _states[16]);
  if(_blocksChanged & 0x00020000) lightBlock(dataBus, U411, _states[17]);
  if(_blocksChanged & 0x00040000) lightBlock(dataBus, U414, _states[18]);
  if(_blocksChanged & 0x00080000) lightBlock(dataBus, U418, _states[19]);
  if(_blocksChanged & 0x00100000) lightBlock(dataBus, U419, _states[20]);
  if(_blocksChanged & 0x00200000) lightBlock(dataBus, U412, _states[21]);
  if(_blocksChanged & 0x00400000) lightBlock(dataBus, U415, _states[22]);
  if(_blocksChanged & 0x00800000) lightBlock(dataBus, U420, _states[23]);
  if(_blocksChanged & 0x01000000) lightBlock(dataBus, U421, _states[24]);

  if(_blocksChanged & 0x02000000) lightBlock(dataBus, U520_U521, _states[25]);
  if(_blocksChanged & 0x04000000) lightBlock(dataBus, U522_523, _states[26]);
  if(_blocksChanged & 0x08000000) lightBlock(dataBus, U251, _states[27]);
  if(_blocksChanged & 0x10000000) lightBlock(dataBus, U252, _states[28]);
  if(_blocksChanged & 0x20000000) lightBlock(dataBus, U267_268, _states[29]);
  if(_blocksChanged & 0x40000000) lightBlock(dataBus, U269_270, _states[30]);
  if(_blocksChanged & 0x80000000) lightBlock(dataBus, U271, _states[31]);

  _blocksChanged = 0;
}

void LedManager::light(ID id, bool enable) {
  lightSlider(id, enable);
}

void LedManager::lightSlider(byte id, bool enable) {
  const byte byteId = id / 8;
  const byte mask = 1 << (id % 8);

  if(enable) {
    _states[byteId] |= mask;
  } else {
    _states[byteId] &= ~mask;
  }
  // TODO: what if it's same value?
  _blocksChanged |= (1 << byteId);
}
