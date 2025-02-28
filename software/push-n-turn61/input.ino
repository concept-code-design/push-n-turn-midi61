void init_ENCODER(){
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_BTN, INPUT_PULLUP);
    int valA = digitalRead(ENCODER_A);
    int valB = digitalRead(ENCODER_B);
    encPrevVal_s = (valA << 1) + valB;
}

void encoderUpdate(){
    int valA = digitalRead(ENCODER_A);
    int valB = digitalRead(ENCODER_B);
    encNewVal_s = (valA << 1) + valB;

    int info = lookupTable_s[encPrevVal_s][encNewVal_s];

    if (info == 1) encClockState_s |= (1 << encNewVal_s); // set the bit to 1
    else if (info == -1) encCounterClockState_s |= (1 << encNewVal_s);
    else if (info == 2) {} // do nothing 

    if (encPrevVal_s != encNewVal_s && encNewVal_s == 3){        // changed to the non moving state, lets figure out what direction we went!
        // for each clockwise and counterclockwise, the encoder state goes through 4 distinct states
        // make sure it's gone through at least 3 of those (and assume if one is missing it's because I didn't read fast enough)
        if (encClockState_s == 0b1011 || encClockState_s == 0b1101 || encClockState_s == 0b1110 || encClockState_s == 0b1111){
        	encValue += encStepSize;
        	if (encValue > 127) encValue = 127; //cw
          sendCtlChange(MIDI_CHANNEL, DATA[7], encValue);
          NEO_Wheel(ENCODER_NEO,encValue,1 );
        }
        if (encCounterClockState_s == 0b1011 || encCounterClockState_s == 0b1101 || encCounterClockState_s == 0b1110 || encCounterClockState_s == 0b1111){
        	encValue -= encStepSize;
        	if (encValue < 0) encValue = 0; //ccw
          sendCtlChange(MIDI_CHANNEL, DATA[7], encValue);
          NEO_Wheel(ENCODER_NEO,encValue,1);
        }
        encClockState_s = 0;
        encCounterClockState_s = 0;
    }
    encPrevVal_s = encNewVal_s;
}

void switchUpdate(){
   // Read and debounce the state of each switch
  for (int i = 0; i < NUM_SWITCHES; i++) {
    int reading = digitalRead(SWITCHES[i]);
    if (reading != lastSwitchState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      if (reading != switchState[i]) {
        switchState[i] = reading;
        if (switchState[i] == LOW) {
          handleSwitch(i);
        } else if ((switchState[i] == HIGH)  && (currentMode == PLAY_MODE)) { // && (lastSwitchState[i] == LOW)
          if (TYPE[i] == NOTE) midiNoteOff(i); else if (TYPE[i] == CC_MOMENTARY) midiCCoff(i);
        }
      }
    }
    lastSwitchState[i] = reading;
  }
}

void handleSwitch(int switchIndex) {
  switch (TYPE[switchIndex]) {
    case NOTE:
        midiNoteOn(switchIndex);
      break;
    case CC_TOGGLE:
      toggleState[switchIndex] = !toggleState[switchIndex];
      if (toggleState[switchIndex] == 1) midiCCon(switchIndex, CTOGGLE); else midiCCoff(switchIndex);
      break;
    case CC_MOMENTARY:
      midiCCon(switchIndex, CMOMENTARY);
      break;
    default:
      break;
  }
}