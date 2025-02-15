# Push-n-Turn 61

Push-n-Turn 61 is a USB MIDI controller with 6 keyboard switches (RedDragon low profile), a rotary encoder and 7 RGB leds (WS8212).

The device sports a USB type-C connector and requires no device drivers on Mac or PC.

The keys can be configured (via MIDI messages, or the configurator) to produce notes, CC codes (momentary or toggle mode). The encoder produces 0..127 on a configurable CC channel. The encoder button produces 127/0 on a CC channel of choice.
Each switch has an RGB led that shows the state of the switch.
The device uses a modified midi library for CH55x devices made by mi:muz 

## Configuring the keys
Configuration of the keys can be done by sending midi codes to the device.
You can use python midiutil for this (https://github.com/sourcebox/midiutil).
The module listens to CC codes on channel 10 (0xBA on midiutil)

start configuration
0xBA, 0x64, 0x2A

end configuration and store data in EEprom
0xBA, 0x64, 0x2B

programming switches uses the following format
prefix (185) type+switch note/value
type is
	1: note mode
	2: CC momentary mode
	3: CC toggle mode
switch is 1..6
type and switch are combined into a number with first position for type, second for switch. So 12 is switch 2 in note mode, 35 is switch 5 in CC toggle mode
note/value is the note or CC code associated to the switch (C3 (middle C) is 60).

./midiutil.py -d 1 -w 185 11 60
will configure switch one to produce a C3 when pressed.

0xBA 0x64 0x2C
produces device type 0xBA 0x64 0x61 in [61, 31, 40, 51, F4] for 6key+1rot, you get the idea
 

0xBA 0x64 0x2D
returns the firmware revision: 0xBA, 0x64, 0x10 for instance for rev 1.0



## Links & References

* [mi:muz:ch55x Tiny USB MIDI Library for Ch55xduino](https://github.com/mimuz/mimuz-ch55x)
* []()

# License

![OpenSourceLicense](https://github.com/push-n-turn-midi61/OpenSourceLicense.png)

Concept & Code invests time and resources providing this open source design, please support them and open-source hardware by purchasing products from Etsy or Tindie!

Designed by Ronald Leenes.

Firmware released under an GNU AGPL v3.0 license. See the LICENSE file for more information.

Hardware released under an CERN Open Hardware Licence v1.2. See the LICENSE_HARDWARE file for more information.


February 2025
