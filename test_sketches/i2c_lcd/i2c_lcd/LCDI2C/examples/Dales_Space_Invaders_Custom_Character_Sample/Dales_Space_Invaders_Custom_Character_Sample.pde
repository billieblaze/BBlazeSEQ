/*
  i2c Simple lcd customer characters sample
  
  Assumes 2 line by 16 char display
  
   Dale Wentz 3-27-2009
 
 
 */


#include <Wire.h>
#include <LCDI2C.h>
#include "macros.h"


//unsigned char cc0[8];
//unsigned char cc1[8];
//unsigned char cc2[8];
//unsigned char cc3[8];
//unsigned char cc4[8];
//unsigned char cc5[8];
//unsigned char cc6[8];
//unsigned char cc7[8];

    LCDI2C lcd = LCDI2C(2,16,0x4C,1);

void setup()
{

  //    Note custom char 0 does not work on version 1.3 or 1.4 display
  //    Define Custom Characters 0 - 7
  
  uint8_t cc0[8] = { 
    B8(01110),
    B8(10001),
    B8(01110),
    B8(00100),
    B8(00100),
    B8(01110),
    B8(10001),
    B8(01110)
};

  uint8_t cc1[8] = { 
    B8(10001),
    B8(01110),
    B8(10101),
    B8(01110),
    B8(10001),
    B8(10001),
    B8(00000),
    B8(00000)
};

  uint8_t cc2[8] = {  
    B8(10001),
    B8(01110),
    B8(10101),
    B8(01110),
    B8(01010),
    B8(01010),
    B8(00000),
    B8(00000)
};

  uint8_t cc3[8] = { 
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000)
  };

 uint8_t cc4[8] = {  
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00100),
    B8(01110),
    B8(11111)
 };


 uint8_t cc5[8] = { 
    B8(00100),
    B8(00100),
    B8(00100),
    B8(00100),
    B8(00000),
    B8(00100),
    B8(01110),
    B8(11111)
 };
 
  uint8_t cc6[8] = {
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00100),
    B8(00100),
    B8(00100),
    B8(00100)
  };
  
  uint8_t cc7[8] = { 
    B8(00100),
    B8(00100),
    B8(00100),
    B8(00100),
    B8(00000),
    B8(00000),
    B8(00000),
    B8(00000)
};

 

  lcd.init();          // Init the display
  
  
  
  lcd.load_custom_character(1,cc0);      // send custom characters to the display
  delay(50);
  lcd.load_custom_character(1,cc1);
  delay(50);
  lcd.load_custom_character(2,cc2);
  delay(50);
  lcd.load_custom_character(3,cc3);
  delay(50);
  lcd.load_custom_character(4,cc4);
  delay(50);
  lcd.load_custom_character(5,cc5);
  delay(50);
  lcd.load_custom_character(6,cc6);
  delay(50);
  lcd.load_custom_character(7,cc7);
  delay(50);
  
  lcd.clear();                          // clear the display after sending characters. Cursor goes to odd place after uploading
  
  lcd.print("Hello World!");            // Print hello world and customer characters
  lcd.write(0x00);                      // Custom Char 0 (Does not work on my display)
  lcd.write(0x01);
  lcd.write(0x02);
  lcd.write(0x03);
  lcd.write(0x04); 
  lcd.write(0x05);
  lcd.write(0x06);
  lcd.write(0x07);   
  delay(2000);                          // just sleep a bit

  lcd.clear();
  
  
}

void loop()            // Space invaders loop
{

  int j=1;
  int move=1;
  int pos=1;
  int posmove=1;
  
  for(int i=0;i < 2000;i++){
    
      lcd.setCursor(0,pos);
      lcd.print(" ");
      lcd.write(0x01);
      lcd.print(" ");
      
      lcd.setCursor(0,pos);   
      lcd.print(" ");
      lcd.write(0x02);
      lcd.print(" ");
      
      lcd.setCursor(0,pos+4);
      lcd.print(" ");
      lcd.write(0x02);
      lcd.print(" ");
       
      lcd.setCursor(0,pos+4);
      lcd.print(" ");
      lcd.write(0x01);
      lcd.print(" ");
       
       
      if(pos == 0)
        posmove = 1;
      if(pos == 10)
        posmove = -1;
      
      pos = pos + posmove;
      
      if (j == 15)
        move = -1;
      if (j == 0)
        move = 1;
        
      lcd.setCursor(1,j);
      lcd.print(" ");
      lcd.write(0x04);
      lcd.print(" ");
      j = j + move;

      
  }
  


  
}
