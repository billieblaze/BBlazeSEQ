// example use of LCDI2C library with the web4robot.com i2C lcd display

#include <Wire.h>
#include <LCDI2C.h>



LCDI2C lcd = LCDI2C(2,16,0x4C,1);             // Number of lines and i2c address of the display


void setup() { 


  lcd.init();                          // Init the display, clears the display
 
  lcd.print("Hello World!");       // Classic Hello World!
 

  
}


void loop()
{
  
}
