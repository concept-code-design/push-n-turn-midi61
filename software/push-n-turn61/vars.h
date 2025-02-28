// Application
uint8_t VERSION;

// extern __xdata uint8_t ch;
// extern __xdata uint8_t note;
// extern __xdata uint8_t vel;

// hardware
#define INSTRUMENT_TYPE 61 // this is a push & turn 61

#define FW_VERSION    10
#define NUM_SWITCHES   7
#define NUM_ENCODERS   1
#define ENCODER_A     31
#define ENCODER_B     30
#define ENCODER_BTN   33 // also part of the switches array in last position
#define PROGBOOT_BTN  33
#define NEO_COUNT      7
#define NEO_RGB                        // type of pixel: NEO_GRB or NEO_RGB
#define PIN_NEO       34 
#define ENCODER_NEO    6 
static uint8_t SWITCHES[NUM_SWITCHES] = {11, 17, 16, 15, 14, 32, 33};

// type is communicated as the most signifacnt nibble of the 'note' 1(0) being Note, 2(0) being CC_Toggle etc 
// keys run from 1 .. 7, with 7 being encoder 
#define NOTE          1
#define CC_TOGGLE     2
#define CC_MOMENTARY  3
#define ENCODER_R     4
#define MIDICHANNEL   5
#define ENCODERSTEPS  6

// soundControl
__xdata uint8_t TYPE[NUM_SWITCHES] = {0, 0, 0, 0, 0, 0, 3}; // encoder is 6

__xdata uint8_t DATA[NUM_SWITCHES] = {3, 9, 14, 15, 20, 21, 22}; // undefined midi CCs. 3,9, 14, 15, 20-31, 85-87,89,90, 102-119
__xdata uint8_t MIDI_CHANNEL = 12;
__xdata uint8_t VELOCITY = 127;

// blinkenlights
#define LEDBUF (NEO_COUNT*3)
__xdata uint8_t ledData[LEDBUF];
__xdata static int r, g, b;
__xdata static int colors[NUM_SWITCHES];

// household duties
#define DEBOUNCE_DELAY  5         // Debounce time in ms
// Switch states
__xdata  int lastSwitchState[NUM_SWITCHES] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
__xdata  int switchState[NUM_SWITCHES] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
__xdata  bool toggleState[NUM_SWITCHES] = {false, false, false, false, false, false, false};
__xdata  unsigned long lastDebounceTime[NUM_SWITCHES] = {0,0,0,0,0,0,0};  



#define PLAY_MODE 0
#define PROG_MODE 1
#define PROG_DONE 2
__xdata uint8_t currentMode = PLAY_MODE;

// twister stuff
__xdata uint8_t encValue = 0;
__xdata uint8_t encPrevVal_s = 0;
__xdata uint8_t encNewVal_s  = 0;
__xdata uint8_t encStepSize = 1;
__xdata unsigned int encClockState_s = 0;
__xdata unsigned int encCounterClockState_s  = 0;
// lookup table for encoder
__xdata static int lookupTable_s[4][4] = {{0, -1, 1, 2},
                                  {1, 0, 2, -1},
                                  {-1, 2, 0, 1},
                                  {2, 1, -1, 0}};

