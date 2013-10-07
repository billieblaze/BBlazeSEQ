/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO: allow CC data in the pattern - maybe just filter initially? 
// todo: check rotary 3 and 9

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <ChibiOS_AVR.h>
#include <TwiMaster.h>



BSEMAPHORE_DECL(semDIN, 1);


BSEMAPHORE_DECL(semLCD, 1);
//SEMAPHORE_DECL(semMatrix,0);
//SEMAPHORE_DECL(semMIDI,0);

#include "SPI.h"
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include <digitalWriteFast.h>

#include "config.h"  
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h

#include "midi.h"
#include "DIN.h"
#include "keypad.h"


#include "matrix.h"
#include "lcd.h"


// Setup the UI thread
static WORKING_AREA(waScanUI, 64);

static msg_t ScanUI(void *arg) {
  Serial.print("Setup UI");
  setupDIN();
  setupKeyMatrix();
   
  while (1) {
     Serial.println("Scan UI");
     scanDIN();
     scanKeyMatrix();
   
    chThdSleepMilliseconds(180); 

   
  }
  return 0;  
}

static WORKING_AREA(waProcessUI, 64);

static msg_t ProcessUI(void *arg) {
  
   
  while (1) {
    // Wait for signal from thread 2.

    //TODO implement UI semaphore
   chBSemWait(&semDIN);
 
    Serial.println("ProcessUI");
    // do something
   handleNavButtons();  
   handleEncoders(); 
   
   //handleKeyMatrix();
    chBSemSignal(&semLCD);
   
  }
  return 0;  
}


// 64 byte stack beyond task switch and interrupt needs
static WORKING_AREA(waMIDI, 64);

static msg_t doMIDI(void *arg) {
  Serial.println("Setup MIDI");
//  setupMIDI();

  while (1) {
 
//    chSemWait(&semMIDI);
 
    // do something
     // Serial.println("Process MIDI");

    //MIDI.read();

    // Signal thread 1 to turn LED off.
    //chSemSignal(&sem);

  }
  return 0;  
}

// 64 byte stack beyond task switch and interrupt needs
static WORKING_AREA(waLCD, 64);
 
Mutex flag;

static msg_t doLCD(void *arg) {

  setupLCD();
 
  while (TRUE) {
    chBSemWait(&semLCD);

    // do something
     Serial.println("Process LCD");
     updateLCDArray();
     writeToLCD();   
    Serial.println("LCD Updated");
  
  }
  return 0;  
}

static WORKING_AREA(waMatrix, 64);

static msg_t doMatrix(void *arg) {    
 
  Serial.print("Setup Matrix");
  setupLEDMatrix(); 
 
  while (1) {
    // do something
//  chSemWait(&semMatrix);
    
//  handleMatrix();
   
  }
  return 0;  
}


//------------------------------------------------------------------------------
// main thread runs at NORMALPRIO
void chSetup() {
 
  // Mutex for priority inversion
  chMtxInit(&flag);
  
    
  
 //Serial.println("Register MIDI Thread ");
 //   chThdCreateStatic(waMIDI, sizeof(waMIDI),
 //    NORMALPRIO + 5, doMIDI, NULL);

 
  //Serial.println("Register LED Matrix Thread");
   // chThdCreateStatic(waMatrix, sizeof(waMatrix),   NORMALPRIO, doMatrix, NULL);
  
  
  //Serial.println("Register LCD Thread");
   
    
  
    chThdCreateStatic(waLCD, sizeof(waLCD), NORMALPRIO-2, doLCD, NULL);
    
  
  //Serial.println("Register UI Scan Thread");
   chThdCreateStatic(waScanUI, sizeof(waScanUI),
    NORMALPRIO+4, ScanUI, NULL);


  //Serial.println("Register UI Process Thread");
  chThdCreateStatic(waProcessUI, sizeof(waProcessUI),
    NORMALPRIO+1, ProcessUI, NULL);

  //Serial.println("All threads started!");
 chBSemSignal(&semLCD);
 
 while(1) {}

}


//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
 
  chBegin(chSetup);
  // chBegin never returns, main thread continues with mainThread()
  
}

void loop(){
  // not used
}
