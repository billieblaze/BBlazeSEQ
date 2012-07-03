BBlaze-SEQ
==========

Arduino Based Midi Sequencer w/ LCD, Nav Buttons, RGB LED Matrix / Keypad

This is my first Arduino Project, but not my first microcontroller project.   


Platform: 
=========

* SainSmart Arduino Mega 2560 clone
* SainSmart i2c LCD
* MIDI IN / MIDI OUT (6n139)
* MicroChip 24LC256 - 256k RAM 
* SparkFun 4x4 RGB LED / Button Matrix - 74hc595 shift registers for LED - 74hc164/165 for button matrix

* Navigation 
    - using a digital in module from Midibox.org - but again, just some 74hc595
    * 8 rotary encoder / swithces 
    * 5 nav buttons
    * pattern change rotary/encoder

Libraries:
==========

* SainSmart i2c LCD
* ShiftMatrixPWM
* MIDI 
* Timer3 


Roadmap: 
========

* add save / clear / copy pattern
* allow CC data in the pattern
* PCB Design


ChangeLog:
==========

* v0.9 - Split core .ino file into packages (lcd, keypad, matrix, etc) - working on timing (might remove interrupt) and general cleanup of UI handlers
* v0.8 - Added 24LC256 - working on timing of UI routines - added "record+keyboard" mode
* v0.7 - moved the button handling to an interrupt and added "updateLCD" flag which will update on the next loop()
* v0.6 - switch to i2c LCD and arduino mega 2560
* v0.5 - Replace sequence array that come from Cahors / buttons now toggle notes in the array
* v0.4 - Integrate Keypad w/ a Voltage Divider to A5
* v0.3 - Integrate LED Matrix (shiftPWMMatrix + 75hc595)
* v0.2 - LCD / Menu / Nav Buttons / Simple OS   (sainSmart LCD / Keypad)
* v0.1 - basic poc / find libraries

