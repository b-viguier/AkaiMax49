#ifndef AKAIMAX49_LCDMANAGER_H
#define AKAIMAX49_LCDMANAGER_H

#include <Arduino.h>

class DataBus;

class LcdManager {
public:

  enum Dimension {
    NB_ROWS = 4,
    NB_COLS = 20,
  };

  LcdManager();

  void setup(DataBus& dataBus);

  void update(DataBus& dataBus);

  void printL(byte row, byte col, const char* text);
  void printR(byte row, byte col, const char* text);

  void printIntegerL(byte row, byte col, unsigned int value, byte base = 10);
  void printIntegerR(byte row, byte col, unsigned int value, byte base = 10);

  void clear();

private:
  char _text[Dimension::NB_ROWS][Dimension::NB_COLS];
  bool _changed;
};


#endif //AKAIMAX49_LCDMANAGER_H
