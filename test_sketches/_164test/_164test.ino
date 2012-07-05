#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 


// shiftout 
const int keypadOutputClockPin=A5;
const int keypadOutputDataPin=A4;

void setup()
{
  
  
  
 lcd.begin(16, 2);
  lcd.clear();
  
  lcd.print("164 test");
 pinMode(keypadOutputClockPin, OUTPUT); // make the clock pin an output
  pinMode(keypadOutputDataPin , OUTPUT); // make the data pin an output


}

void loop()
{
  lcd.clear();
  
      lcd.print("255");
      shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  255);
      delay(2000);
      lcd.print("0");
           shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  0);
      delay(1000);
  
}

