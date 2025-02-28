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

void midiCCon(uint8_t i, uint8_t color){
  sendCtlChange(MIDI_CHANNEL, DATA[i], VELOCITY);
  indicatorUpdate(i, color, 1); 
}

void midiCCoff(uint8_t i){
  sendCtlChange(MIDI_CHANNEL, DATA[i], 0);
  indicatorUpdate(i, 0, 0);
}

/*
see prog.h for programming protocol

*/
void onCTLChange(uint8_t ch, uint8_t note, uint8_t vel) {
  __xdata uint8_t type = 0;
  __xdata uint8_t key = 0;
   
  ch = getChannel();
  note = getNote();
  vel =  getVel();
  type = note / 10;       // type ranges from 1 – note, 2 - CC_TOGGLE, 3 - CC_MOMENTARY, 4 - ENCODER_R, 5- MIDICHANNEL, 6 - ENCODERSTEPS
  key = (note % 10) - 1;  // we number keys from 1..7 (1..6 are keys, 7 is the encoder button, 8 is encoder rotary) 
// echo back what we got
 // sendNoteOn(1, note, vel); // channel 1 gives the note and veloccity code 
 // sendNoteOn(2, type, key); // channel 2 gives us the inferred type and key
  
  if (currentMode == PROG_MODE) {
    if ((type > 0) && (type < 4)) TYPE[key] = type; // the 6 keys and rotary button can be notes, CC[momentary, toggle]    
    if ((vel >= 0) && (vel < 128)) DATA[key] = vel; 
    if (key == 7) DATA[7] = vel;       // handle the rotary encoder CC
    switch (type) {
      case NOTE:          NEO_writeColor(key, 0, 150, 105);             // violet 122
        break; 
      case CC_TOGGLE:     NEO_writeColor(key,  0, 195, 60);             //   blue 150
        break;
      case CC_MOMENTARY:  NEO_writeColor(key,   96, 0, 159);            //  aqua 202
        break;
      case ENCODER_B:     NEO_writeColor(ENCODER_NEO,   148, 0, 112);   //  turquoise 218
        break; 
      case ENCODER_R:     NEO_writeColor(ENCODER_NEO,   148, 100, 112); //  
        break; 
      case MIDICHANNEL:   //  set MIDI channel
        if (vel > 0 && vel <16) MIDI_CHANNEL = vel;
        eeprom_write_byte(0, MIDI_CHANNEL);
        break;
      case ENCODERSTEPS:  // set encoder stepsize
        if (vel > 0 && vel < 16) encStepSize = vel;
        eeprom_write_byte(1, encStepSize);
        break;
      default: 
        break;
    }
    NEO_update();
  }

  if ((ch == 10) && (note == 102)) {
    if (vel > 127)sendNoteOn(3, note, vel);
    switch (vel){
      case 42:
          currentMode = PROG_MODE; // enter configuration mode
          for (int i = 0; i < NEO_COUNT; i++) NEO_writeColor(i, 0, 240, 15); // RED 90
          NEO_update();
          break;
      case 43:
          //currentMode = PROG_DONE;  //configuration done
          for (int i = 0; i < NEO_COUNT; i++) NEO_writeColor(i, 240, 0, 15); // green2 250
          NEO_update();
          writeEEPROM();
          delay(2000);
          NEO_clearAll();
          currentMode = PLAY_MODE;
          break;
      case 44:                    // return instrument tyoe
          sendCtlChange(10, 103, INSTRUMENT_TYPE);
          break;
      case 45:                    // return firmware version
          sendCtlChange(10, 104, FW_VERSION);
          break;
      case 46:                    // return current config
        for (int i = 0; i < NUM_SWITCHES; i++) sendCtlChange(10, ((TYPE[i]+1)*10)+i+1, DATA[i]);
        sendCtlChange(10, 8, DATA[7]);
         break;
      case 47:                  // return stepsize
        sendCtlChange(10, 105, encStepSize);
        break;
      case 48:                // return MIDI channel
        sendCtlChange(10, 106, MIDICHANNEL);
        break;
      case 49: // reboot in prog mode                            // update pixels
        PROGBOOT_now();     // jump to bootloader
        break;
      default:
        break;
    }
  }
}
