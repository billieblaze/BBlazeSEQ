BBlaze-SEQ
==========

Arduino Based Midi Sequencer w/ LCD, Nav Buttons, RGB LED Matrix / Keypad

This is my first Arduino Project, but not my first microcontroller project.   


Platform: 
=========

* SainSmart Arduino Uno clone
* SainSmart 16x2 LCD + Keypad Shield (will later switch to serial LCD)
* Custom MIDI board w/ basic MIDI IN / MIDI OUT (6n139)
* MicroChip 24LC256 - 256k RAM 
* SparkFun 4x4 RGB LED / Button Matrix 


Libraries:
==========

* SainSmart LCD / Keypad Library
* ShiftMatrixPWM
* MIDI (tho, I'm going to check out Tymms MIDI lib, it's event based!)
* Potentially a Timer (1 or 2?) 


ChangeLog:
==========
* v0.5 - Replace sequence array that come from Cahors / buttons now toggle notes in the array
* v0.4 - Integrate Keypad w/ a Voltage Divider to A5
* v0.3 - Integrate LED Matrix (shiftPWMMatrix + 75hc595)
* v0.2 - LCD / Menu / Nav Buttons / Simple OS   (sainSmart LCD / Keypad)
* v0.1 - basic poc / find libraries

