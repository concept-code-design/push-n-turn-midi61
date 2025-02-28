/* programming documentation

start prog
./midiutil.py -d 1 -w 186 102 42

end prog
./midiutil.py -d 1 -w 186 102 43

module type
./midiutil.py -d 1 -w 186 102 44
returns: (10, 103, INSTRUMENT_TYPE);


firmware version
./midiutil.py -d 1 -w 186 102 45
returns (10, 104, FW_VERSION) 10 for version 1.0

show configuration
./midiutil.py -d 1 -w 186 102 46
returns the config of each of the 6 buttons, the rotary knob and rotary encoder

show stepsize 
./midiutil.py -d 1 -w 186 102 47
returns (10, 105, encStepSize)

show MIDI channel 
./midiutil.py -d 1 -w 186 102 48
returns (10, 106, MIDIChannel)

reboot in firmware update mode
./midiutil.py -d 1 -w 186 102 49

programming keys
./midiutil.py -d 1 -w 186 102 42
./midiutil.py -d 1 -w 186 11 60
./midiutil.py -d 1 -w 186 12 61
./midiutil.py -d 1 -w 186 23 9
./midiutil.py -d 1 -w 186 24 15
./midiutil.py -d 1 -w 186 35 21
./midiutil.py -d 1 -w 186 36 22
./midiutil.py -d 1 -w 186 27 23
./midiutil.py -d 1 -w 186 8 24
./midiutil.py -d 1 -w 186 102 43





*/