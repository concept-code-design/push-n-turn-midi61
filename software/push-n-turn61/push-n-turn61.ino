// ===================================================================================
// Project:   Push-n-Turn (31, 41, 61)
// Version:   v1.0
// Year:      2025
// Author:    Ronald Leenes
// Github:    https://github.com/concept-code-design
// License:   GNU AGPL v3.0 license
// ===================================================================================
//
// Description:
// ------------
// Firmware for the various Push-n-Turn midi controllers building on mimuz-ch55x midi library for CH55x chips
//
// References:
// -----------
// - D.F.Mac. : https://github.com/mimuz/mimuz-ch55x
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn

#include "config.h"
#include "vars.h"
#include "push-n-turn.h"
#include "colors.h"
#include "mimuz-ch55x.h"
#include <WS2812.h>

#ifndef USER_USB_RAM
#error "This sketch needs to be compiled with a USER USB setting"
#endif

void setup(){
 readEEPROM();
  init_PINS();
  init_ENCODER();

  init_NEO();

  check_PROGBOOT_REQUEST();
  USBInit();

 // setHdlNoteOn(onNoteOn);
 // setHdlNoteOff(onNoteOff);
  setHdlCtlChange(onCTLChange);
}

void loop() {
  switchUpdate();
  encoderUpdate();
  
  // if (NEO_dirty && (millis() > NEO_reset)){                 
  //   if (colors[ENCODER_NEO] == 0) NEO_clearPixel(ENCODER_NEO); else NEO_Wheel(ENCODER_NEO, colors[ENCODER_NEO]);
    
  //  // NEO_update();
  //   NEO_dirty = false;
  // }
  processMidiMessage();
  USBSerial_flush();
}
