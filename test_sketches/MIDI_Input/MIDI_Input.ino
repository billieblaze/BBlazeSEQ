#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DFR_Key.h>
#include <MIDI.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

DFR_Key keypad;

/*
  MIDI Input tutorial
  by Franky
  28/07/2009
  
  NOTE: for easier MIDI input reading, 
  take a look a the Callbacks example.
  
*/




void setup() {
      lcd.init();                      // initialize the lcd 
  lcd.backlight();
lcd.print("WELCOME");
  MIDI.begin();            	// Launch MIDI with default options
				// (input channel is default set to 1)
}

void loop() {

  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    lcd.setCursor(0,1);
    
    switch(MIDI.getType()) {		// Get the type of the message we caught
      case ProgramChange:               // If it is a Program Change
	lcd.print(MIDI.getData1());	// Blink the LED a number of times 
					// correponding to the program number 
					// (0 to 127, it can last a while..)
        break;
      case ControlChange:
    lcd.print("CC: ");
    lcd.print(MIDI.getData1());	
    lcd.print(" -  ");    
    lcd.print(MIDI.getData2());
    lcd.print("     ");

      // See the online reference for other message types
      
      default:
      
    
        break;
    }
  }
}
