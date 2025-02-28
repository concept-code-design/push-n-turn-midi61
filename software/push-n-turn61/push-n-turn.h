
//function prototypes
// mimuz library
extern uint8_t getChannel();
extern uint8_t getNote();
extern uint8_t getVel();
 extern __xdata uint8_t ch;
 extern __xdata uint8_t note;
 extern __xdata uint8_t vel;

void PROGBOOT_now();
void check_PROGBOOT_REQUEST();
void init_PINS();
void init_NEO();
void NEO_clearAll();
void NEO_update();
void NEO_clearPixel(int);
void NEO_writeColor(uint8_t, uint8_t,uint8_t,uint8_t);
void set_pixel_for_RGB_LED(uint8_t, uint8_t,uint8_t,uint8_t,uint8_t);
void indicatorUpdate(uint8_t pixel, uint8_t color, uint8_t state);

void NEO_Wheel(int, int, bool);

void midiNoteOn(uint8_t);
void midiNoteOff(uint8_t);
void midiCCon(uint8_t, uint8_t);
void midiCCoff(uint8_t);

void onCTLChange(uint8_t ch, uint8_t note, uint8_t vel);

void init_ENCODER();
void encoderUpdate();
void signalEncoderChange(uint8_t value);

void switchUpdate();
//void checkModeSwitching(int key1, int key2);
void handleSwitch(int switchIndex);
void readEEPROM();
void writeEEPROM();