#include "ButtonManager.h"
#include "LedManager.h"
#include "LcdManager.h"
#include "Pins.h"
#include "DataBus.h"

static_assert(4 == sizeof(unsigned int), "32 bits integers required");

LedManager ledManager;
ButtonManager buttonManager;
LcdManager lcdManager;
DataBus dataBus;

#define DEFAULT_CALLBACK(ID) buttonManager.setCallback(ButtonManager::ID,[](bool enable) { ledManager.light(LedManager::ID, enable); })

void setup() {

  Serial.begin(9600);

  // Waiting power
  delay(1000);

  Serial.println("Bus setup…");
  dataBus.setup();

  Serial.println("LED setup…");
  ledManager.setup(dataBus);

  Serial.println("Buttons setup…");
  buttonManager.setup();

  Serial.println("LCD setup…");
  lcdManager.setup(dataBus);


  DEFAULT_CALLBACK(NOTE_REPEAT);
  DEFAULT_CALLBACK(LATCH);
  DEFAULT_CALLBACK(ON_OFF);
  DEFAULT_CALLBACK(SEQ_SELECT);
  DEFAULT_CALLBACK(ARP_MODE);
  DEFAULT_CALLBACK(PAD_BANK_A);
  DEFAULT_CALLBACK(PAD_BANK_B);
  DEFAULT_CALLBACK(PAD_BANK_C);
  DEFAULT_CALLBACK(PAD_BANK_D);
  DEFAULT_CALLBACK(FULL_LEVEL);
  DEFAULT_CALLBACK(TEMPO);
  DEFAULT_CALLBACK(OCT_DOWN);
  DEFAULT_CALLBACK(OCT_UP);
  DEFAULT_CALLBACK(MODE_MIDI_CC);
  DEFAULT_CALLBACK(MODE_SEQ_CC);
  DEFAULT_CALLBACK(MODE_SEQ_PITCH);
  DEFAULT_CALLBACK(BANK_1_8);
  DEFAULT_CALLBACK(BANK_9_16);
  DEFAULT_CALLBACK(BANK_17_24);
  DEFAULT_CALLBACK(BANK_25_32);
  DEFAULT_CALLBACK(TIME_DIV);

  buttonManager.setCallback(ButtonManager::TIME_4,[](bool enable) { ledManager.light(LedManager::TIME_4_R, enable); ledManager.light(LedManager::TIME_4_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_4T,[](bool enable) { ledManager.light(LedManager::TIME_4T_R, enable); ledManager.light(LedManager::TIME_4T_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_8,[](bool enable) { ledManager.light(LedManager::TIME_8_R, enable); ledManager.light(LedManager::TIME_8_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_8T,[](bool enable) { ledManager.light(LedManager::TIME_8T_R, enable); ledManager.light(LedManager::TIME_8T_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_16,[](bool enable) { ledManager.light(LedManager::TIME_16_R, enable); ledManager.light(LedManager::TIME_16_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_16T,[](bool enable) { ledManager.light(LedManager::TIME_16T_R, enable); ledManager.light(LedManager::TIME_16T_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_32,[](bool enable) { ledManager.light(LedManager::TIME_32_R, enable); ledManager.light(LedManager::TIME_32_G, enable); });
  buttonManager.setCallback(ButtonManager::TIME_32T,[](bool enable) { ledManager.light(LedManager::TIME_32T_R, enable); ledManager.light(LedManager::TIME_32T_G, enable); });

  buttonManager.setCallback(ButtonManager::SHIFT,[](bool enable) { ledManager.light(LedManager::PAD1_UP, enable);});
  buttonManager.setCallback(ButtonManager::EDIT_STORE,[](bool enable) { ledManager.light(LedManager::PAD2_DOWN, enable);});
  buttonManager.setCallback(ButtonManager::PROGRAM_STD,[](bool enable) { ledManager.light(LedManager::PAD3_INC, enable);});
  buttonManager.setCallback(ButtonManager::GLOBAL_HUI,[](bool enable) { ledManager.light(LedManager::PAD4_EXCL, enable);});
  buttonManager.setCallback(ButtonManager::PROG_CHANGE,[](bool enable) { ledManager.light(LedManager::PAD5_RAND, enable);});
  buttonManager.setCallback(ButtonManager::PREV,[](bool enable) { ledManager.light(LedManager::PAD6_CHORD, enable);});
  buttonManager.setCallback(ButtonManager::NEXT,[](bool enable) { ledManager.light(LedManager::PAD7_DOUBLE, enable);});
  buttonManager.setCallback(ButtonManager::STOP,[](bool enable) { ledManager.light(LedManager::PAD8_PATTERN, enable);});
  buttonManager.setCallback(ButtonManager::PLAY,[](bool enable) { ledManager.light(LedManager::PAD9, enable);});
  buttonManager.setCallback(ButtonManager::RECORD,[](bool enable) { ledManager.light(LedManager::PAD10, enable);});
  buttonManager.setCallback(ButtonManager::LEFT,[](bool enable) { ledManager.light(LedManager::PAD11, enable);});
  buttonManager.setCallback(ButtonManager::RIGHT,[](bool enable) { ledManager.light(LedManager::PAD12, enable);});

  lcdManager.printL(0, 0, "Akai");
  lcdManager.printR(0, LcdManager::Dimension::NB_COLS-1, "Akai");
  lcdManager.printL(1, 0, "Max49");
  lcdManager.printR(1, LcdManager::Dimension::NB_COLS-1, "Max49");
  lcdManager.printIntegerR(2, LcdManager::Dimension::NB_COLS-1, 123456);
  lcdManager.printIntegerL(2, 0, 123456);
  lcdManager.printIntegerL(3, 0, 0);
  lcdManager.printIntegerR(3, 19, 123);
  Serial.println("Init Done");
}

byte prevLeds[8] = {0};
unsigned int previousTime = 0;
void loop() {


  for(unsigned int i = 0; i< LedManager::NB_LED_PER_SLIDER; ++i) {
    ledManager.lightSlider(LedManager::SLIDER_1+prevLeds[0], false);
    ledManager.lightSlider(LedManager::SLIDER_2+prevLeds[1], false);
    ledManager.lightSlider(LedManager::SLIDER_3+prevLeds[2], false);
    ledManager.lightSlider(LedManager::SLIDER_4+prevLeds[3], false);
    ledManager.lightSlider(LedManager::SLIDER_5+prevLeds[4], false);
    ledManager.lightSlider(LedManager::SLIDER_6+prevLeds[5], false);
    ledManager.lightSlider(LedManager::SLIDER_7+prevLeds[6], false);
    ledManager.lightSlider(LedManager::SLIDER_8+prevLeds[7], false);

    const unsigned int m = millis();
    for(unsigned j=0; j<8; ++j) {
      prevLeds[j] = (int)(LedManager::NB_LED_PER_SLIDER * (sin(m/100.0 + j*0.5)+1) / 2);
    }

    ledManager.lightSlider(LedManager::SLIDER_1+prevLeds[0], true);
    ledManager.lightSlider(LedManager::SLIDER_2+prevLeds[1], true);
    ledManager.lightSlider(LedManager::SLIDER_3+prevLeds[2], true);
    ledManager.lightSlider(LedManager::SLIDER_4+prevLeds[3], true);
    ledManager.lightSlider(LedManager::SLIDER_5+prevLeds[4], true);
    ledManager.lightSlider(LedManager::SLIDER_6+prevLeds[5], true);
    ledManager.lightSlider(LedManager::SLIDER_7+prevLeds[6], true);
    ledManager.lightSlider(LedManager::SLIDER_8+prevLeds[7], true);


    const unsigned int time = millis() / 1000;
    if(previousTime != time) {
      previousTime = time;
      lcdManager.printIntegerL(3, 0, time);
    }

    ledManager.update(dataBus);
    buttonManager.update(dataBus);
    lcdManager.update(dataBus);

    delay(5);
  }
}
