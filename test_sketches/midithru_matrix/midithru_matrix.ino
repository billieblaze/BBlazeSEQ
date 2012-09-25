
#include <Wire.h>

#include <MIDI.h>
#include "SPI.h"
#include <LiquidCrystal_I2C.h>


// ShiftMatrixPWM handles the led matrix and connects to a group of 3 daisy chained 74hc595's to control RGB (columns) and a seperate 74hc595 (rows)
  //Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
  //Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
    const int ShiftMatrixPWM_columnLatchPin=49;
    const int ShiftMatrixPWM_rowDataPin=A15;
    const int ShiftMatrixPWM_rowClockPin=A14;
    const int ShiftMatrixPWM_rowLatchPin=A13;
    
    const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.
    const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.
    
    unsigned char maxBrightness = 1;
    unsigned char pwmFrequency = 10;
    
    int numColumnRegisters = 3; 
    int numRows=4;
    
    int numColumns = numColumnRegisters*8;
    int numOutputs = numColumns*numRows;

#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


/*
  MIDI Input tutorial
 by Franky
 28/07/2009
 
 NOTE: for easier MIDI input reading, 
 take a look a the Callbacks example.
 
 */

void HandleStart(){ 
  //Serial.print("Start");
} 

void HandleClock(){ 
 // Serial.print("Clock");
} 

void HandleStop(){ 
  //Serial.print("Stop");
} 

void setup() {
 // Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  
  lcd.backlight();
  
  delay(500);
  lcd.print("WELCOME");
  
  // MATRIX 
   //Setup LED Matrix
  pinMode(ShiftMatrixPWM_columnLatchPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowDataPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowClockPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowLatchPin, OUTPUT); 

  SPI.setBitOrder(LSBFIRST);
  // SPI_CLOCK_DIV2 is only a tiny bit faster in sending out the last byte. 
  // SPI transfer and calculations overlap for the other bytes.
  SPI.setClockDivider(SPI_CLOCK_DIV2); 
  SPI.begin(); 

  ShiftMatrixPWM.SetMatrixSize(numRows, numColumnRegisters);
  ShiftMatrixPWM.Start(pwmFrequency,maxBrightness);  
 ShiftMatrixPWM.SetAll(0);

  
  // MIDI
   MIDI.begin();            	// Launch MIDI with default options
    MIDI.setHandleStart(HandleStart);
 MIDI.setHandleClock(HandleClock);
 MIDI.setHandleStop(HandleStop);
MIDI.setHandleContinue(HandleStart);

    MIDI.turnThruOn();
  
  //Serial.print("Start");
 
  // (input channel is default set to 1)
}

void loop() {
 // Serial.print("read: ");
  //Serial.print(millis());
  MIDI.read(); 
 //Serial.print(" - Complete \n");

}

