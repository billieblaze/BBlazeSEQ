// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []       i2c LCD library keypad Test Demo
// []	
// []	4-2-2009  dale@wentztech.com
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


#define VERSION "1.1"

#include <Wire.h>

//  Uncomment the include for the display that you are using


#include <LCDi2cW.h>                    //   web4robot.com
LCDi2cW lcd = LCDi2cW(2,16,0x4C,0);             // Number of lines and i2c address of the display

//#include <LCDi2cR.h>                       //   robot-electronics.co.uk 
//LCDi2cR lcd = LCDi2cR(4,40,0x63,0);



void setup()
{ 

  lcd.init();                           // Init the display, clears the display
 
  lcd.print("Keypad Ver ");             // Show Version
  lcd.print (VERSION);

  lcd.setCursor(1,0);                   // Place the cursor on the 2nd line  
  lcd.print("Keypad = ");
  
}


void loop()
{
  
 int keyInput;

  keyInput=lcd.keypad();        // Get value from keypad
  
  lcd.setCursor(1,10);        
  lcd.print("   ");             // Clear prev entry
  
  lcd.setCursor(1,10);
  lcd.print(keyInput,DEC);      // Print the value we got back from the keypad
                     
  delay(10);
  
  
}

