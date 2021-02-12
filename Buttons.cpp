#include <Arduino.h>

#include "Buttons.h"
#include "Pins.h"

typedef void (*ButtonCallback)(bool);

void onButtonChangedDefault(bool new_state) {
  if(new_state) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button released");
  }
}

#define CREATE_CALLBACK(ID) ([](bool new_state) { \
  Serial.print(ID); \
  if(new_state) { \
    Serial.println(" pressed"); \
  } else { \
    Serial.println(" released"); \
  } \
  })


byte triggerButton(byte state, Pin pin, byte mask, ButtonCallback callback) {
  bool pinValue;
  if( (pinValue = digitalRead(pin)) != (bool)(state & mask) ) {
    callback(!pinValue);
    return mask;
  }

  return 0;
}


byte triggerBlockButtons(ButtonBlockAddr blockAddr, byte &state
    , ButtonCallback callback1
    , ButtonCallback callback2
    , ButtonCallback callback3
    , ButtonCallback callback4
    , ButtonCallback callback5
    , ButtonCallback callback6
    , ButtonCallback callback7
    , ButtonCallback callback8
    ) {

  digitalWrite(PIN_ADDR_10, blockAddr & 0b00000001);
  digitalWrite(PIN_ADDR_11, blockAddr & 0b00000010);
  digitalWrite(PIN_ADDR_12, blockAddr & 0b00000100);
  digitalWrite(PIN_ADDR_13, blockAddr & 0b00001000);
  digitalWrite(PIN_ADDR_14, blockAddr & 0b00010000);
  digitalWrite(PIN_ADDR_15, blockAddr & 0b00100000);

  delay(10);
  digitalWrite(PIN_CLOCK_17, LOW);
  delay(10);
  digitalWrite(PIN_CLOCK_17, HIGH);

  return state ^ (
      triggerButton(state, PIN_DATA_1, 0b00000001, callback1) |
      triggerButton(state, PIN_DATA_2, 0b00000010, callback2) |
      triggerButton(state, PIN_DATA_3, 0b00000100, callback3) |
      triggerButton(state, PIN_DATA_4, 0b00001000, callback4) |
      triggerButton(state, PIN_DATA_5, 0b00010000, callback5) |
      triggerButton(state, PIN_DATA_6, 0b00100000, callback6) |
      triggerButton(state, PIN_DATA_7, 0b01000000, callback7) |
      triggerButton(state, PIN_DATA_8, 0b10000000, callback8)
                 );
}


void triggerButtons() {
  static byte buttonsState[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

  for (byte pin = PIN_DATA_1; pin <= PIN_DATA_8; ++pin) {
    pinMode(pin, INPUT);
  }

  buttonsState[0] = triggerBlockButtons(U524, buttonsState[0]
      , CREATE_CALLBACK("TIME_4")
      , CREATE_CALLBACK("TIME_4T")
      , CREATE_CALLBACK("TIME_8")
      , CREATE_CALLBACK("TIME_8T")
      , CREATE_CALLBACK("TIME_16")
      , CREATE_CALLBACK("TIME_16T")
      , CREATE_CALLBACK("TIME_32")
      , CREATE_CALLBACK("TIME_32T")
    );
 
  buttonsState[1] = triggerBlockButtons(U221, buttonsState[1]
      , CREATE_CALLBACK("BANK_1-8")
      , CREATE_CALLBACK("BANK-9-16")
      , CREATE_CALLBACK("BANK-17-24")
      , CREATE_CALLBACK("BANK-25-32")
      , CREATE_CALLBACK("MODE_PITCH")
      , CREATE_CALLBACK("MODE-CC")
      , CREATE_CALLBACK("MODE-MIDI-CC")
      , CREATE_CALLBACK("TIME_DIV")
  );
  buttonsState[2] = triggerBlockButtons(U220, buttonsState[2]
      , CREATE_CALLBACK("UP")
      , CREATE_CALLBACK("DOWN")
      , CREATE_CALLBACK("LEFT")
      , CREATE_CALLBACK("RIGHT")
      , CREATE_CALLBACK("EDIT STORE")
      , CREATE_CALLBACK("PRG STANDARD")
      , CREATE_CALLBACK("GLOBAL HUI")
      , CREATE_CALLBACK("PRG-CHANGE")
  );
  buttonsState[3] = triggerBlockButtons(U223, buttonsState[3]
      , CREATE_CALLBACK("PREV")
      , CREATE_CALLBACK("STOP")
      , CREATE_CALLBACK("PLAY") // !!
      , CREATE_CALLBACK("REC")  // !!
      , CREATE_CALLBACK("NEXT") // !!
      , CREATE_CALLBACK("SHIFT") // !!
      , CREATE_CALLBACK("TEMPO")
      , CREATE_CALLBACK("ENTER")
  );
  buttonsState[4] = triggerBlockButtons(U222, buttonsState[4]
      , CREATE_CALLBACK("OCT DOWN")
      , CREATE_CALLBACK("OCT UP")
      , CREATE_CALLBACK("PLAY") // !!
      , CREATE_CALLBACK("REC")  // !!
      , CREATE_CALLBACK("NEXT") // !!
      , CREATE_CALLBACK("SHIFT")
      , CREATE_CALLBACK("ARP")
      , CREATE_CALLBACK("SEQ SEL")
  );
  buttonsState[5] = triggerBlockButtons(U272, buttonsState[5]
      , CREATE_CALLBACK("ON-OFF")
      , CREATE_CALLBACK("LATCH")
      , CREATE_CALLBACK("NOTE REPEAT")
      , CREATE_CALLBACK("PAD A")
      , CREATE_CALLBACK("PAD B")
      , CREATE_CALLBACK("PAD C")
      , CREATE_CALLBACK("PAD D")
      , CREATE_CALLBACK("FULL LEVEL")
  );
 
}
