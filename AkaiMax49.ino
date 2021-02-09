

#define PIN_DATA_1 23
#define PIN_DATA_2 24
#define PIN_DATA_3 25
#define PIN_DATA_4 26
#define PIN_DATA_5 27
#define PIN_DATA_6 28
#define PIN_DATA_7 29
#define PIN_DATA_8 30

#define PIN_ADDR_15 40
#define PIN_ADDR_14 39
#define PIN_ADDR_13 38
#define PIN_ADDR_12 35
#define PIN_ADDR_11 34
#define PIN_ADDR_10 33

#define PIN_CLOCK_17 44
#define PIN_CLOCK_16 43

#define PIN_ENABLE_19 48

enum LedBlockAddr {
  U310 = 0b01000111,
  U312 = 0b01001000,
  U316 = 0b01001001,
  U317 = 0b01001010,
  U311 = 0b01001011,
  U313 = 0b01001100,
  U319 = 0b01001101,
  U318 = 0b01001110,
  U314 = 0b01001111,
  U315 = 0b01010000,
  U320 = 0b01010001,
  U321 = 0b01010010,
  U410 = 0b01010011,
  U413 = 0b01010100,
  U416 = 0b01010101,
  U417 = 0b01010110,
  U422 = 0b01010111,
  U411 = 0b01011000,
  U414 = 0b01011001,
  U418 = 0b01011010,
  U419 = 0b01011011,
  U415 = 0b01011100,
  U420 = 0b01011101,
  U421 = 0b01011110,
  U412 = 0b01011111,
  U520_U521 = 0b01000000,
  U522_523 = 0b01000001,
  U251 = 0b01000010,
  U252 = 0b01000011,
  U267_268 = 0b01000100,
  U269_270 = 0b01000101,
  U271 = 0b01000110,
};


void lightBlock(byte addr, byte data)
{
  digitalWrite(PIN_ADDR_10, addr & 0b00000001);
  digitalWrite(PIN_ADDR_11, addr & 0b00000010);
  digitalWrite(PIN_ADDR_12, addr & 0b00000100);
  digitalWrite(PIN_ADDR_13, addr & 0b00001000);
  digitalWrite(PIN_ADDR_14, addr & 0b00010000);
  digitalWrite(PIN_ADDR_15, addr & 0b00100000);

  digitalWrite(PIN_DATA_1, data & 0b00000001);
  digitalWrite(PIN_DATA_2, data & 0b00000010);
  digitalWrite(PIN_DATA_3, data & 0b00000100);
  digitalWrite(PIN_DATA_4, data & 0b00001000);
  digitalWrite(PIN_DATA_5, data & 0b00010000);
  digitalWrite(PIN_DATA_6, data & 0b00100000);
  digitalWrite(PIN_DATA_7, data & 0b01000000);
  digitalWrite(PIN_DATA_8, data & 0b10000000);

  delay(10);
  digitalWrite(PIN_CLOCK_16, LOW);
  delay(10);
  digitalWrite(PIN_CLOCK_16, HIGH);
}

void snake(byte addr)
{
  for(byte led = 1; led > 0; led = led << 1) {
    lightBlock(addr, led);
    delay(10);
  }
  lightBlock(addr, 0);
}


void setup() {
  
  Serial.begin(9600);
  
  pinMode(PIN_ENABLE_19, OUTPUT); digitalWrite(PIN_ENABLE_19, HIGH);

  for (byte pin = PIN_DATA_1; pin <= PIN_DATA_8; ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  for (byte pin = PIN_ADDR_10; pin <= PIN_ADDR_15; ++pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  pinMode(PIN_CLOCK_16, OUTPUT); digitalWrite(PIN_CLOCK_16, HIGH);
  pinMode(PIN_CLOCK_17, OUTPUT); digitalWrite(PIN_CLOCK_17, HIGH);

  Serial.println("Waiting 1s");
  delay(1000);

  Serial.println("Light down");
  lightBlock(U310, 0);
  lightBlock(U312, 0);
  lightBlock(U316, 0);
  lightBlock(U317, 0);
  lightBlock(U311, 0);
  lightBlock(U313, 0);
  lightBlock(U319, 0);
  lightBlock(U318, 0);
  lightBlock(U314, 0);
  lightBlock(U315, 0);
  lightBlock(U320, 0);
  lightBlock(U321, 0);
  lightBlock(U410, 0);
  lightBlock(U413, 0);
  lightBlock(U416, 0);
  lightBlock(U417, 0);
  lightBlock(U422, 0);
  lightBlock(U411, 0);
  lightBlock(U414, 0);
  lightBlock(U418, 0);
  lightBlock(U419, 0);
  lightBlock(U415, 0);
  lightBlock(U420, 0);
  lightBlock(U421, 0);
  lightBlock(U412, 0);
  lightBlock(U520_U521, 0);
  lightBlock(U522_523, 0);
  lightBlock(U251, 0);
  lightBlock(U252, 0);
  lightBlock(U267_268, 0);
  lightBlock(U269_270, 0);
  lightBlock(U271, 0);
  
  digitalWrite(PIN_ENABLE_19, LOW);
  Serial.println("Init Done");
}

void loop() {
  
  Serial.println("loop");
  
 
  snake(U310);
  snake(U312);
  snake(U316);
  snake(U317);
  snake(U311);
  snake(U313);
  snake(U318);
  snake(U319);
  snake(U314);
  snake(U315);
  snake(U320);
  snake(U321);
  snake(U410);
  snake(U413);
  snake(U416);
  snake(U417);
  snake(U422);
  snake(U411);
  snake(U414);
  snake(U418);
  snake(U419);
  snake(U412);
  snake(U415);
  snake(U420);
  snake(U421);
  
}
