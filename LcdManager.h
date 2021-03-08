#ifndef AKAIMAX49_LCDMANAGER_H
#define AKAIMAX49_LCDMANAGER_H

#include <Arduino.h>

class LcdManager {
public:

  enum Dimension {
    NB_ROWS = 4,
    NB_COLS = 20,
  };

  LcdManager();

  void setup();

  void update();

  void printL(byte row, byte col, const char* text);
  void printR(byte row, byte col, const char* text);

  void printIntegerL(byte row, byte col, unsigned int value);
  void printIntegerR(byte row, byte col, unsigned int value);

  void clear();

private:
  char _text[Dimension::NB_ROWS][Dimension::NB_COLS];
  bool _changed;
};


#endif //AKAIMAX49_LCDMANAGER_H
