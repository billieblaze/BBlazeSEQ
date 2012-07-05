/*
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */
 
#include "TimerOne.h"
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
int counter = 0;

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Timer1 Test");
  
  
  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}
 
void callback()
{
  lcd.setCursor(0,1);
  lcd.print("INT-");
  lcd.print(counter);
  counter++;
  
}
 
void loop()
{
  // your program here...
}
