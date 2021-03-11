#ifndef AKAIMAX49_DATABUS_H
#define AKAIMAX49_DATABUS_H

#include <Arduino.h>

class DataBus {
public:

  void setup();

  enum Mode {
    BUS_IN = INPUT,
    BUS_OUT = OUTPUT,
  };

  void setMode(Mode mode);

  void setAddress(byte addr) const;

  void write(byte data) const;

  byte read() const;

private:
  Mode _mode = Mode::BUS_IN;
};

#endif //AKAIMAX49_DATABUS_H
