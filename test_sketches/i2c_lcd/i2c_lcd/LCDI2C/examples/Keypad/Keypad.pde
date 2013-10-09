// example use of LCDI2C library with the web4robot.com i2C lcd display

#include <Wire.h>
#include <LCDI2C.h>


LCDI2C lcd = LCDI2C(2,16,0x4C,1);             // Number of lines and i2c address of the display


void setup() { 


  lcd.init();                          // Init the display, clears the display
 
  lcd.print("Hello World!");             // Classic Hello World!
 
  lcd.setCursor(1,0);                      // Place the cursor on the 2nd line
  
  lcd.print("Keypad = ");
  
}


void loop()
{
  
 int keyInput;

  keyInput=lcd.keypad();
  
  lcd.setCursor(1,10);
  
  lcd.print("   ");      // Clear prev entry
  
  lcd.setCursor(1,10);
  
  lcd.print(keyInput,DEC);// Print the value we got back from the keypad
                     
  delay(10);
  
  
}

