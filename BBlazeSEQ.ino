/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO: allow CC data in the pattern - maybe just filter initially? 
// todo: check rotary 3 and 9

#include <stdlib.h>
#include <Arduino.h>
#include <ChibiOS_AVR.h>
#include "constants.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MIDI.h>
#include <digitalWriteFast.h>
#include "config.h" 
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h
#include "midi.h"
#include "DIN.h"
#include "matrix.h"
#include "lcd.h"

// main thread runs at NORMALPRIO
void chSetup() {
   
 //Serial.println("Register MIDI Thread ");
 //   chThdCreateStatic(waMIDI, sizeof(waMIDI),
 //    NORMALPRIO + 5, doMIDI, NULL);

  Serial.println("Register UI Scan Thread");
   chThdCreateStatic(waScanUI, sizeof(waScanUI),NORMALPRIO+3, ScanUI, NULL);

  Serial.println("Register UI Process Thread");
  chThdCreateStatic(waProcessUI, sizeof(waProcessUI),NORMALPRIO+1, ProcessUI, NULL);

  Serial.println("Register Matrix Thread");
  chThdCreateStatic(waProcessMatrix, sizeof(waProcessMatrix),NORMALPRIO+2, ProcessMatrix, NULL);

  Serial.println("Register LCD Thread");
  chThdCreateStatic(waLCD, sizeof(waLCD), NORMALPRIO, doLCD, NULL);   

 while(1) {}

}

void setup() {
  Serial.begin(9600);
  setupLEDMatrix(); 
  handleLEDMatrix();
  setupLCD();
  setupDIN();
  setupKeyMatrix();
  
  //chBegin(chSetup);  // chBegin never returns, main thread continues with mainThread()
}

void loop(){} // not used
