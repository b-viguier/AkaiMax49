#ifndef AKAIMAX49_PADMANAGER_H
#define AKAIMAX49_PADMANAGER_H

#include <Arduino.h>

class PadManager {

public:
  PadManager();

  void setup();

  void update();

  typedef void (*Callback)(byte id, byte value);

  PadManager& setCallback(Callback callback);

  enum {
    NB_COLS = 4,
    NB_ROWS = 3,
    NB_PADS = NB_COLS * NB_ROWS,
  };

private:
  Callback _callback;
  byte _values[NB_PADS];

};


#endif //AKAIMAX49_PADMANAGER_H
