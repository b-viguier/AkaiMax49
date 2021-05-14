#ifndef AKAIMAX49_ROTARYMANAGER_H
#define AKAIMAX49_ROTARYMANAGER_H

#include <Arduino.h>

class RotaryManager {

public:
  RotaryManager();

  void setup();

  void update();

  typedef void (*Callback)(int count);

  RotaryManager& setCallback(Callback callback);

private:
  Callback _callback;
};


#endif //AKAIMAX49_ROTARYMANAGER_H
