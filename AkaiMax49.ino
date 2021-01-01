

enum Status {
  BUTTON1 = 0b01,
  BUTTON2 = 0b10,

  OFF = 0b00,
  ON = BUTTON1 | BUTTON2,
  PUSHING = BUTTON1,
};

struct KeyState {
  bool button1 = false;
  bool button2 = false;
  unsigned long pushStart = 0;
};

#define KOUT_FIRST_PIN 2
#define KOUT_NB 2
#define KOUT_LAST_PIN (KOUT_FIRST_PIN + KOUT_NB - 1)

#define KIN_FIRST_PIN 8
#define KIN_NB 5
#define KIN_LAST_PIN (KIN_FIRST_PIN + KIN_NB - 1)

#define NB_KEYS (KIN_NB * KOUT_NB / 2)
#define KPINS_TO_KEY_INDEX(kin, kout) ((kout - KOUT_FIRST_PIN - kout%2) * 4 + kin - KIN_FIRST_PIN)

// C3
#define NOTE_FIRST 48
#define NOTE_DELTA_MIN 5
#define NOTE_DELTA_MAX 60


KeyState states[NB_KEYS];

void sendNoteOn(byte keyIndex, unsigned int buttonsDelta) {
  digitalWrite(13, HIGH);
  //Serial.print("delta:");
  //Serial.println(buttonsDelta);
  //Serial.print("0X90|");
  //Serial.print(NOTE_FIRST + keyIndex, HEX);
  //Serial.print("|");
  Serial.write(0x90);
  Serial.write(NOTE_FIRST + keyIndex);
  if (buttonsDelta < NOTE_DELTA_MIN) {
    //Serial.println("0x7F");
    Serial.write(0x7F);
  } else if (buttonsDelta > NOTE_DELTA_MAX) {
    //Serial.println("0x00");
    Serial.write(0x00);
  } else {
    //Serial.println((byte)(0x7F - 0x7F * (buttonsDelta - NOTE_DELTA_MIN + 0.0f) / (NOTE_DELTA_MAX - NOTE_DELTA_MIN)), HEX);
    Serial.write((byte)(0x7F - 0x7F * (buttonsDelta - NOTE_DELTA_MIN + 0.0f) / (NOTE_DELTA_MAX - NOTE_DELTA_MIN)));
  }
}

void sendNoteOff(byte keyIndex) {
  //Serial.print("0X80|");
  Serial.write(0x80);
  //Serial.println(NOTE_FIRST + keyIndex, HEX);
  Serial.write(NOTE_FIRST + keyIndex);
  Serial.write(0x00);
  digitalWrite(13, LOW);
}

void setup() {
  Serial.begin(31250);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  for (byte pin = KIN_FIRST_PIN; pin <= KIN_LAST_PIN;
       ++pin) {
    pinMode(pin, INPUT_PULLUP);
  }
  for (byte pin = KOUT_FIRST_PIN; pin <= KOUT_LAST_PIN;
       ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

void loop() {

  for (byte koutPin = KOUT_FIRST_PIN; koutPin <= KOUT_LAST_PIN; ++koutPin) {
    // First key button
    digitalWrite(koutPin, LOW);
    for (byte kinPin = KIN_FIRST_PIN; kinPin <= KIN_LAST_PIN; ++kinPin) {
      const byte keyIndex = KPINS_TO_KEY_INDEX(kinPin, koutPin);
      KeyState &state = states[keyIndex];
      
      const bool pressed = !digitalRead(kinPin);
      if (state.button2 != pressed) {
        if (pressed) {
          //Serial.println("button2 Pressed");
          
          sendNoteOn(keyIndex, millis() - state.pushStart);
        } else {
          //Serial.println("button2 Released");
          sendNoteOff(keyIndex);
        }
        state.button2 = pressed;
      }
    }
    digitalWrite(koutPin, HIGH);

    // Second key button
    ++koutPin;
    digitalWrite(koutPin, LOW);
    for (byte kinPin = KIN_FIRST_PIN; kinPin <= KIN_LAST_PIN; ++kinPin) {
      KeyState &state = states[KPINS_TO_KEY_INDEX(kinPin, koutPin)];
      const bool pressed = !digitalRead(kinPin);
      if (state.button1 != pressed) {
        if (pressed) {
          //Serial.println("button1 Pressed");
          state.pushStart = millis();
        } else {
          //Serial.println("button1 Released");
        }
        state.button1 = pressed;
      }
    }
    digitalWrite(koutPin, HIGH);
  }
}
