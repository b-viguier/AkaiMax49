#ifndef AKAIMAX49_BUTTONMANAGER_H
#define AKAIMAX49_BUTTONMANAGER_H


class ButtonManager {
public:
  ButtonManager();

  void setup();

  void update();


  enum ID {
    NOTE_REPEAT,
    LATCH,
    ON_OFF,
    SEQ_SELECT,
    ARP_MODE,

    PAD_BANK_A,
    PAD_BANK_B,
    PAD_BANK_C,
    PAD_BANK_D,
    FULL_LEVEL,

    SHIFT,
    EDIT_STORE,
    PROGRAM_STD,
    GLOBAL_HUI,
    PROG_CHANGE,
    TEMPO,

    PREV,
    NEXT,
    STOP,
    PLAY,
    RECORD,

    OCT_DOWN,
    OCT_UP,

    LEFT,
    RIGHT,
    UP,
    DOWN,
    ENTER,

    MODE_MIDI_CC,
    MODE_SEQ_CC,
    MODE_SEQ_PITCH,

    BANK_1_8,
    BANK_9_16,
    BANK_17_24,
    BANK_25_32,

    TIME_DIV,
    TIME_4,
    TIME_4T,
    TIME_8,
    TIME_8T,
    TIME_16,
    TIME_16T,
    TIME_32,
    TIME_32T,
  };

  typedef void (*Callback)(bool);

  ButtonManager& setCallback(ID id, Callback callback);




private:
  byte _states[6];

  Callback _callbacks[6*8];
};


#endif //AKAIMAX49_BUTTONMANAGER_H
