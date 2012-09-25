
#include <Wire.h>

#include <MIDI.h>

#include <LiquidCrystal_I2C.h>


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
  /Serial.print(" - Complete \n");

}

