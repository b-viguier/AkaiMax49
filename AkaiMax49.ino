
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // TODO: invert
  pinMode(2, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
}

enum Status {
  BUTTON1 = 0b01,
  BUTTON2 = 0b10,

  OFF = 0b00,
  ON = BUTTON1 | BUTTON2,
  PUSHING = BUTTON1,
};

byte state = OFF;
unsigned long pushStart = 0;

void loop() {

  const byte val1 = digitalRead(8);
  const byte val2 = digitalRead(9);
  const byte newState = (val1 ? 0 : BUTTON1) | (val2 ? 0 : BUTTON2);

  if( newState != state) {
    Serial.println("new state");
    Serial.println(newState);

    if(state == OFF && newState == PUSHING) {
      Serial.println("Pushing…");
      pushStart = millis();
    } else if(state == PUSHING && newState == ON) {
      Serial.println("On!!");
      Serial.println(millis() - pushStart);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      Serial.println("other…");
      digitalWrite(LED_BUILTIN, LOW);
    }
   
    state = newState;
  }
}
