/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO:  integrate control knobs and rework the ui for modifying params

// TODO: allow CC data in the pattern - maybe just filter initially? 

#include "constants.h"
#include <Wire.h>
#include "SPI.h"
#include "hsv2rgb.h"
#include <TimerThree.h>
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h
#include "lcd.h"
#include "matrix.h"
#include "keypad.h"
#include "navkeys.h"
#include "midi.h"

// Setup
void setup(){
  setupLCD();
  setupLEDMatrix(); 
  setupKeypad();
  setupMIDI();
  lcd.clear();
 // handleLCD();

  // start the keypad scanning
  Timer3.initialize(200000); 
  Timer3.attachInterrupt( pollUI ); 
    
}

// Main Loop
void loop(){

  MIDI.read();
  handleKeypad();
  handleMatrix();
  
  if (updateLCD == 1){
    handleLCD();
    updateLCD = 0;
  }
}

void pollUI(){
  scanKeypad();
  handleNavKeys(); 
}
