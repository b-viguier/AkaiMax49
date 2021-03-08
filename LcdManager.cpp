#include "LcdManager.h"
#include "Pins.h"

/**
 * DISCLAIMER
 * This code is copied from LiquidCrystal library,
 * adapted for this specific project.
 */

namespace {
  enum Command {
    LCD_CLEARDISPLAY = 0x01,
    LCD_RETURNHOME = 0x02,
    LCD_ENTRYMODESET = 0x04,
    LCD_DISPLAYCONTROL = 0x08,
    LCD_CURSORSHIFT = 0x10,
    LCD_FUNCTIONSET = 0x20,
    LCD_SETCGRAMADDR = 0x40,
    LCD_SETDDRAMADDR = 0x80,
  };

  enum EntryFlag {
    LCD_ENTRYRIGHT = 0x00,
    LCD_ENTRYLEFT = 0x02,
    LCD_ENTRYSHIFTDECREMENT = 0x00,
    LCD_ENTRYSHIFTINCREMENT = 0x01,
  };

  enum OnOffFlag {
    LCD_DISPLAYON = 0x04,
    LCD_DISPLAYOFF = 0x00,
    LCD_CURSORON = 0x02,
    LCD_CURSOROFF = 0x00,
    LCD_BLINKON = 0x01,
    LCD_BLINKOFF = 0x00,
  };

  enum CursorFlag {
    LCD_DISPLAYMOVE = 0x08,
    LCD_CURSORMOVE = 0x00,
    LCD_MOVERIGHT = 0x04,
    LCD_MOVELEFT = 0x00,
  };

  enum FunctionFlag {
    LCD_8BITMODE = 0x10,
    LCD_4BITMODE = 0x00,
    LCD_2LINE = 0x08,
    LCD_1LINE = 0x00,
    LCD_5x10DOTS = 0x04,
    LCD_5x8DOTS = 0x00,
  };

  enum LCD {
    DISPLAY_FUNCTION = LCD_8BITMODE | LCD_2LINE | LCD_5x8DOTS,
    DISPLAY_CONTROL = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF,
    ROW_OFFSET_0 = 0x00,
    ROW_OFFSET_1 = 0x40,
    ROW_OFFSET_2 = ROW_OFFSET_0 + LcdManager::NB_COLS,
    ROW_OFFSET_3 = ROW_OFFSET_1 + LcdManager::NB_COLS,

  };


/************ low level data pushing commands **********/

  void pulseEnable(void) {
    digitalWrite(PIN_LCD_ENABLE_22, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_LCD_ENABLE_22, HIGH);
    delayMicroseconds(1);    // enable pulse must be >450ns
    digitalWrite(PIN_LCD_ENABLE_22, LOW);
    delayMicroseconds(100);   // commands need > 37us to settle
  }

  void write8bits(uint8_t value) {

    digitalWrite(PIN_DATA_1, value & 0x01);
    digitalWrite(PIN_DATA_2, value & 0x02);
    digitalWrite(PIN_DATA_3, value & 0x04);
    digitalWrite(PIN_DATA_4, value & 0x08);
    digitalWrite(PIN_DATA_5, value & 0x10);
    digitalWrite(PIN_DATA_6, value & 0x20);
    digitalWrite(PIN_DATA_7, value & 0x40);
    digitalWrite(PIN_DATA_8, value & 0x80);
    for (int i = 0; i < 8; i++) {
      digitalWrite(PIN_DATA_1 + i, (value >> i) & 0x01);
    }

    pulseEnable();
  }

  // write either command or data, with automatic 4/8-bit selection
  void send(byte value, byte mode) {
    digitalWrite(PIN_LCD_RS_24, mode);
    digitalWrite(PIN_LCD_RW_23, LOW);

    write8bits(value);
  }

  /*********** mid level commands, for sending data/cmds */

  inline void command(byte value) {
    send(value, LOW);
  }

  inline void write(byte value) {
    send(value, HIGH);
  }

  void sendLineToLCD(char* currentChar, const uint8_t rowOffset) {
      bool isCursorAtTheRightPlace = false;
      for(uint8_t index = 0; index < LcdManager::NB_COLS; ++index, ++currentChar) {
      if(*currentChar == 0) {
        isCursorAtTheRightPlace = false;
        continue;
      }

      if(!isCursorAtTheRightPlace) {
        command(LCD_SETDDRAMADDR | (rowOffset + index));
        isCursorAtTheRightPlace = true;
      }

      write(*currentChar);
      *currentChar = 0;
    }
  }
}

LcdManager::LcdManager()
: _text{}
, _changed(true)
{
}

void LcdManager::setup() {

  // Contrast
  analogWrite(PIN_LCD_V0_25, 128);

  pinMode(PIN_LCD_RS_24, OUTPUT);
  pinMode(PIN_LCD_RW_23, OUTPUT);
  pinMode(PIN_LCD_ENABLE_22, OUTPUT);
  for (byte pin = PIN_DATA_1; pin <= PIN_DATA_8; ++pin) {
    pinMode(pin, OUTPUT);
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delayMicroseconds(50000);
  // Now we pull both RS and R/W low to begin commands
  digitalWrite(PIN_LCD_RS_24, LOW);
  digitalWrite(PIN_LCD_ENABLE_22, LOW);
  digitalWrite(PIN_LCD_RW_23, LOW);

  //put the LCD into 4 bit or 8 bit mode

    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);
    delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);
    delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);


  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | DISPLAY_FUNCTION);

  // turn the display on with no cursor or blinking default
  command(LCD_DISPLAYCONTROL | DISPLAY_CONTROL);

  // clear it off
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!

  // Initialize to default text direction (for romance languages)
  command(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
}

void LcdManager::update() {
  if(!_changed) {
    return;
  }

  for (byte pin = PIN_DATA_1; pin <= PIN_DATA_8; ++pin) {
    pinMode(pin, OUTPUT);
  }

  sendLineToLCD(&_text[0][0], LCD::ROW_OFFSET_0);
  sendLineToLCD(&_text[1][0], LCD::ROW_OFFSET_1);
  sendLineToLCD(&_text[2][0], LCD::ROW_OFFSET_2);
  sendLineToLCD(&_text[3][0], LCD::ROW_OFFSET_3);

  Serial.println("LCD Updated");
  _changed = false;
}

void LcdManager::printL(byte row, byte col, const char* text) {
  for(;*text;++text,++col) {
    _text[row][col] = *text;
  }
  _changed = true;
}

void LcdManager::printR(byte row, byte col, const char* text) {
  // Find last character
  byte nbChar = 0;
  for(;*text;++text,++nbChar);
  for(--text;nbChar;--col,--nbChar,--text) {
    _text[row][col] = *text;
  }
  _changed = true;
}

void LcdManager::printIntegerL(byte row, byte col, unsigned int value) {
  // Find number length
  byte len = 0;
  auto valueCopy = value;
  do {
    valueCopy /= 10;
    ++len;
  } while(valueCopy);
  for(col+=len-1;len;--len,--col,value/=10) {
    _text[row][col] =  '0' + (value % 10);
  }
  _changed = true;
}

void LcdManager::printIntegerR(byte row, byte col, unsigned int value) {
  do {
    _text[row][col] = '0' + (value % 10);
    value /= 10;
    --col;
  } while(value);
  _changed = true;
}

void LcdManager::clear()
{
  for(byte row=0; row<Dimension::NB_ROWS; ++row) {
    for(byte col=0; col<Dimension::NB_COLS; ++col) {
      _text[row][col] = ' ';
    }
  }
  _changed = true;
}
