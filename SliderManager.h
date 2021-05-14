#ifndef AKAIMAX49_SLIDERMANAGER_H
#define AKAIMAX49_SLIDERMANAGER_H

#include <Arduino.h>

class SliderManager {

public:
  SliderManager();

  void setup();

  void update();

  typedef void (*Callback)(byte id, byte value);

  SliderManager& setCallback(Callback callback);

  enum {
    NB_SLIDERS = 8,
  };

private:
  Callback _callback;
  byte _values[NB_SLIDERS];
};


#endif //AKAIMAX49_SLIDERMANAGER_H
