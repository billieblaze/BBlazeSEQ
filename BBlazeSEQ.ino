/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO: allow CC data in the pattern - maybe just filter initially? 
// todo: check rotary 3 and 9

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

#include <Wire.h>

#include "SPI.h"

#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include <digitalWriteFast.h>

#include "config.h"
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h



#include "midi.h"

#include "keypad.h"
#include "navkeys.h"
#include "encoders.h"
#include "UI.h"


#include "matrix.h"
#include "lcd.h"


// Setup
void setup(){
  //Serial.begin(9600);
  setupLCD();
  setupLEDMatrix(); 
  setupKeypad();
  setupMIDI();
 }

// Main Loop
void loop(){
 startTime = millis();
 MIDI.read();
 scanUI();
 handleMatrix();
 handleEncoders(); 
 handleNavButtons(); 
 handleKeypad();
 updateLCDArray();
 writeToLCD(); 
}
