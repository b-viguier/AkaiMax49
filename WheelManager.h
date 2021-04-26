#ifndef AKAIMAX49_WHEELMANAGER_H
#define AKAIMAX49_WHEELMANAGER_H

#include <Arduino.h>

class WheelManager {

public:
  void setup();

  void update();

  typedef void (*Callback)(byte);

  enum ID {
    PITCH,
    MOD,
  };

  WheelManager& setCallback(ID id, Callback callback);

private:

  byte _lastValues[2];
  Callback _callbacks[2];

};


#endif //AKAIMAX49_WHEELMANAGER_H
