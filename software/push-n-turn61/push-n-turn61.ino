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
  if (NUM_ENCODERS > 0) init_ENCODER();

  init_NEO();

  check_PROGBOOT_REQUEST();
  USBInit();
  setHdlCtlChange(onCTLChange);
}

void loop() {
  switchUpdate();
  if (NUM_ENCODERS > 0) encoderUpdate();
  processMidiMessage();
  USBSerial_flush();
}
