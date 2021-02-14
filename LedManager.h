#ifndef AKAIMAX49_LEDMANAGER_H
#define AKAIMAX49_LEDMANAGER_H

class LedManager {

public:


  enum SlideId {
    NB_LED_PER_SLIDER = 25,

    BEGIN = 0,
    SLIDER_1 = BEGIN,
    SLIDER_2 = SLIDER_1 + NB_LED_PER_SLIDER,
    SLIDER_3 = SLIDER_2 + NB_LED_PER_SLIDER,
    SLIDER_4 = SLIDER_3 + NB_LED_PER_SLIDER,
    SLIDER_5 = SLIDER_4 + NB_LED_PER_SLIDER,
    SLIDER_6 = SLIDER_5 + NB_LED_PER_SLIDER,
    SLIDER_7 = SLIDER_6 + NB_LED_PER_SLIDER,
    SLIDER_8 = SLIDER_7 + NB_LED_PER_SLIDER,

    END = SLIDER_8 + NB_LED_PER_SLIDER,
  };
  
  enum ID {

    TIME_4_G = END,
    TIME_4T_G,
    TIME_8_G,
    TIME_8T_G,
    TIME_16_G,
    TIME_16T_G,
    TIME_32_G,
    TIME_32T_G,

    TIME_4_R,
    TIME_4T_R,
    TIME_8_R,
    TIME_8T_R,
    TIME_16_R,
    TIME_16T_R,
    TIME_32_R,
    TIME_32T_R,

    BANK_1_8,
    BANK_9_16,
    BANK_17_24,
    BANK_25_32,


    OCT_DOWN,
    OCT_UP,

    TIME_DIV = OCT_UP+3,
    MODE_SEQ_PITCH,
    MODE_SEQ_CC,
    MODE_MIDI_CC,
    TEMPO,

    PAD9 = TEMPO+4,
    PAD10,
    PAD11,
    PAD12,
    PAD5_RAND,
    PAD6_CHORD,
    PAD7_DOUBLE,
    PAD8_PATTERN,
    PAD1_UP,
    PAD2_DOWN,
    PAD3_INC,
    PAD4_EXCL,


    PAD_BANK_A,
    PAD_BANK_B,
    PAD_BANK_C,
    PAD_BANK_D,
    FULL_LEVEL,

    ARP_MODE,
    SEQ_SELECT,
    ON_OFF,
    LATCH,
    NOTE_REPEAT,


    LAST = NOTE_REPEAT,
  };
  
  LedManager();

  void setup();

  void update();

  void light(ID id, bool enable);

  void lightSlider(byte id, bool enable);

private:

  byte _states[32];
  unsigned int _blocksChanged;
};


#endif //AKAIMAX49_LEDMANAGER_H
