#include "mimuz-ch55x.h"
//#include "vars.h"
//#include <WS2812.h>

void midiNoteOn(uint8_t i){
  sendNoteOn(MIDI_CHANNEL, DATA[i], VELOCITY); 
  indicatorUpdate(i, CCLICK, 1);
}

void midiNoteOff(uint8_t i){
  sendNoteOff(MIDI_CHANNEL, DATA[i]);
  indicatorUpdate(i, 0, 0);
}

void midiCCon(uint8_t i){
  sendCtlChange(MIDI_CHANNEL, DATA[i], VELOCITY);
  indicatorUpdate(i, CTOGGLE, 1);
}

void midiCCoff(uint8_t i){
  sendCtlChange(MIDI_CHANNEL, DATA[i], 0);
  indicatorUpdate(i, 0, 0);
}





// void onNoteOn(uint8_t ch, uint8_t note, uint8_t vel) {
//   // DATA[currentKey] = note;
//   // eeprom_write_byte(currentKey+2, note);
//   // sendNoteOn(ch,note,vel);  // loopback
// }

// void onNoteOff(__xdata uint8_t ch, __xdata uint8_t note) {
//   sendNoteOff(ch,note); // loopback
// }


/*
programming protocol
enter prog mode
key 1: type 1 note: 60 (C4)
key 2: type 2 CC 9
key 3: type 3 CC 15
encoder key: 7 type 2 cc 22
encoder: 7 type: 4 CC 21
end prog
./midiutil.py -d 1 -w 0xBA 0x64 0x2a 
./midiutil.py -d 1 -w 185 11 60
./midiutil.py -d 1 -w 185 12 61
./midiutil.py -d 1 -w 185 13 62
./midiutil.py -d 1 -w 185 14 63
./midiutil.py -d 1 -w 185 15 64
./midiutil.py -d 1 -w 185 16 65
./midiutil.py -d 1 -w 185 27 21
./midiutil.py -d 1 -w 185 47 62
./midiutil.py -d 1 -w 0xBA 0x64 0x2b
*/
void onCTLChange(uint8_t ch, uint8_t note, uint8_t vel) {
  __xdata uint8_t type = 0;
  __xdata uint8_t key = 0;
   
  ch = getChannel();
  note = getNote();
  vel =  getVel();
  type = note / 10;       // type ranges from 1 – note, 2 - CC_TOGGLE, 3 - CC_MOMENTARY, 4 - ENCODER
  key = (note % 10) - 1;  // we number keys from 1..7 (1..6 are keys, 7 is the encoder) 
// echo back what we got
  sendNoteOn(1, note, vel); // channel 1 gives the note and veloccity code 
  sendNoteOn(2, type, key); // channel 2 gives us the inferred type and key
  
  if (currentMode == PROG_MODE) {
    // handle encoder, how?
    // handle regular keys
    if ((type > 0) && (type < 5)) TYPE[key] = type;
    if ((vel >= 0) && (vel <= 127)) DATA[key] = vel;
    switch (type) {
      case 0:    NEO_writeColor(key, 0, 150, 105);      // violet 122
        break; 
      case 1:   NEO_writeColor(key,  0, 195, 60);    //   blue 150
        break;
      case 2:   NEO_writeColor(key,   96, 0, 159); //  aqua 202
        break;
      default: 
        break;
    }
    NEO_update();
  }

  if ((ch == 10) && (note == 100)) {
    switch (vel){
      case 42:
          currentMode = PROG_MODE; // enter configuration mode
          for (int i = 0; i < NEO_COUNT; i++) NEO_writeColor(i, 0, 240, 15); // RED 90
          NEO_update();
          break;
      case 43:
          currentMode = PROG_DONE;  //configuration done
          for (int i = 0; i < NEO_COUNT; i++) NEO_writeColor(i, 240, 0, 15); // green2 250
          NEO_update();
          writeEEPROM();
          delay(2000);
          NEO_clearAll();
          currentMode = PLAY_MODE;
          break;
      case 44:                    // return instrument tyoe
          sendCtlChange(MIDI_CHANNEL, 100, INSTRUMENT_TYPE);
          break;
      case 45:                    // return firmware version
          sendCtlChange(MIDI_CHANNEL, 100, FW_VERSION);
          break;
      case 46:                    // return current config
        for (int i = 0; i < NUM_SWITCHES; i++) sendCtlChange(10, (TYPE[i]*10)+i, DATA[i]);
         break;
      default:
        break;
    }
  }
}
