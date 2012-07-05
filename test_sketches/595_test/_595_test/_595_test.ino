//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************
#include <LiquidCrystal.h>
#include <DFR_Key.h>

// LCD Keys
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
DFR_Key keypad;
int localKey = 0; 
String keyString = "";

//Pin connected to ST_CP of 74HC595
int latchPin = A2;
//Pin connected to SH_CP of 74HC595
int clockPin = A3;
////Pin connected to DS of 74HC595
int dataPin = A4;



void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
   lcd.begin(16, 2);
  lcd.clear();
  lcd.print("595 Test");
  
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
  
    lcd.clear();
    lcd.print(numberToDisplay);
  
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);

    // pause before next value:
    delay(2500);
  }
}

