
void check_PROGBOOT_REQUEST(){
    // Go in bootloader mode if connected with encoder button pressed
  if (!digitalRead(PROGBOOT_BTN))//  && !digitalRead(SWITCHES[0]) && !digitalRead(SWITCHES[2]))
  {
    for (int i = 0 ; i < NEO_COUNT; i++ )      NEO_Wheel(i, CRIMSON, 0);
    NEO_update();                              // update pixels
    PROGBOOT_now();     // jump to bootloader
  }
}

void PROGBOOT_now(void){
  USB_CTRL = 0;
  EA = 0;
  TMOD = 0;
  __asm lcall #BOOT_LOAD_ADDR __endasm;
}

void init_PINS(){
  for (uint8_t i = 0; i < NUM_SWITCHES; i++) {
    pinMode(SWITCHES[i], INPUT_PULLUP);
  }
}

void init_NEO(){
  pinMode(PIN_NEO, OUTPUT);
  digitalWrite(PIN_NEO, LOW);
}

void NEO_clearAll(){
  for (int i = 0; i < NEO_COUNT; i++) NEO_writeColor(i, 0,0,0);
  NEO_update(); 
}

void NEO_clearPixel(int pixel){
  NEO_writeColor(pixel, 0,0,0);
  NEO_update();
}

void NEO_writeColor(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b){
    set_pixel_for_RGB_LED(ledData, pixel, r,g,b);
}

void NEO_update(){
  neopixel_show_P3_4(ledData, LEDBUF);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void NEO_Wheel(int pixel, int WheelPos, bool update) {
  //hues[pixel] = WheelPos;
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    r = 255 - WheelPos * 3;
    g = 0;
    b = WheelPos * 3;
    //NEO_writeColor(pixel, 255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    r = 0;
    g = WheelPos * 3;
    b = 255 - WheelPos * 3;
    //NEO_writeColor(pixel, 0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
  WheelPos -= 170;
  r = WheelPos * 3;
  g = 255 - WheelPos * 3;
  b = 0;
  }
  //NEO_writeColor(pixel, WheelPos * 3, 255 - WheelPos * 3, 0);  
  NEO_writeColor(pixel, r, g, b);
   if (update) NEO_update();
}

// void signalEncoderChange(uint8_t value){
//   NEO_Wheel(ENCODER_NEO, encValue, 1);
//   //NEO_update();
//   // NEO_dirty = true;
//   // NEO_reset = millis() + 500;
// }

void indicatorUpdate(uint8_t pixel, uint8_t color, uint8_t state){
  if (state == 1){
    colors[pixel] = color;
    NEO_Wheel(pixel, color, 1);
    //NEO_update();
  } else if (state == 0){
    colors[pixel] = 0;
    NEO_clearPixel(pixel);          
  }
}

void readEEPROM(){
  MIDI_CHANNEL = eeprom_read_byte(0);
  for (int i = 0; i < 7; i++) {
    TYPE[i] = eeprom_read_byte(1+i);
    DATA[i] = eeprom_read_byte(8+i);
  }
}

void writeEEPROM(){
  eeprom_write_byte(0, MIDI_CHANNEL);
  for (int i = 0; i < 7; i++) {
    eeprom_write_byte(1+i, TYPE[i]);
    eeprom_write_byte(8+i, DATA[i]);
  }
}