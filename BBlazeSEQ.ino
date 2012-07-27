/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO: allow CC data in the pattern - maybe just filter initially? 
// todo: check rotary 3 and 9

#include "constants.h"
#include <stdlib.h> 
#include <Wire.h>
#include "SPI.h"
#include "hsv2rgb.h"
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>

#include "config.h"
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h
#include "lcd.h"
#include "matrix.h"
#include "keypad.h"
#include "encoders.h"
#include "navkeys.h"
#include "midi.h"
#include "UI.h"



// Setup
void setup(){
  setupLCD();
  setupLEDMatrix(); 
  setupKeypad();
  setupMIDI();
}

// Main Loop
void loop(){

  MIDI.read();
  handleMatrix();
 scanUI();
  if(millis() % 3 == 0){ 
    updateLCDArray();
    
  }

  if(millis() % 7 == 0){ 
     scanKeypad();
     handleUI(); 
  }

  if(millis() % 11 == 0){ 
   writeToLCD();  
  }

}
