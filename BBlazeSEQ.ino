/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO: allow CC data in the pattern - maybe just filter initially? 
// todo: check rotary 3 and 9

#include "constants.h"
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

void scanUI(){
   byte bitVal;
  byte bytesVal;
  
  digitalWrite(navploadPin, LOW);
  digitalWrite(navploadPin, HIGH);

  for ( int row = 0; row < 4; row++){
    bytesVal = 0;
    
    for(int i = 0; i < 8; i++){
       bitVal = PINA & ( 1 << PA0 );
      bytesVal |= (bitVal  << (7 - i));

      digitalWrite(navclockPin, HIGH);
      digitalWrite(navclockPin, LOW);
    } 

    newNavKeyValues[row]=bytesVal;
  }
}

// Setup
void setup(){
  setupLCD();
  setupLEDMatrix(); 
  setupKeypad();
  setupMIDI();

  handleLCD();
  handleMatrix();
  
}

// Main Loop
void loop(){

   // Important stuff first
  MIDI.read();
  
  // Inputs
  if (millis() %2 == 0){ 
    scanUI();
  }
  
  scanKeypad();
  
  handleEncoders();
  handleNavButtons();
  handleKeypad();
  
  // Outputs
  handleMatrix();
  handleLCD();
}
