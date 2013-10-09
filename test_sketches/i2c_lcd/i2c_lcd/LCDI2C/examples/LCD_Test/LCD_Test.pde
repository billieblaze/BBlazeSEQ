// example use of LCDI2C library with the web4robot.com i2C lcd display

#include <Wire.h>
#include <LCDI2C.h>

int g_rows = 4;
int g_cols = 20;

LCDI2C lcd = LCDI2C(g_rows,g_cols,0x4C,1);             // Number of lines and i2c address of the display



void setup() { 

  lcd.init();                          // Init the display, clears the display
 
  lcd.print("Hello World!");       // Classic Hello World!
 
   delay(1000);
   BackLight(0);
   delay(5000);
   BackLight(50);
  lcdtest();
}


void loop()
{

}


void lcdtest(){
  
  version();
  delay(2000);
  Cursor_Test();
  Characters();
  Every_Line(g_rows);
  Every_Pos(g_rows,g_cols);
  Fancy_Clear();
   // Count_Numbers();
   // Fancy_Clear();


}


void Cursor_Test(){

  lcd.setCursor(0,0);
  lcd.print("Cursor Test Line");
  lcd.cursor_on();
  lcd.setCursor(0,0);

  for(int i=0 ; i < g_cols -1 ; i++){
      lcd.right();
      delay(500);
  }
  
    lcd.setCursor(0,0);
    lcd.print("Cursor Test Blnk");
    lcd.setCursor(0,g_cols-1);
    lcd.blink_on();
    
    for(int i=0 ; i < g_cols -1 ; i++){
      lcd.left();
      delay(500);
  }
  delay(1000);
  lcd.blink_off();
  lcd.cursor_off();
  
  
  
}





void Count_Numbers(){

  lcd.clear();
  lcd.print("Count to 255");

  for (int i=0;i<255;i++){
    
    lcd.setCursor(1,0);
    
    lcd.print(i,DEC);
    
    lcd.setCursor(1,7);
    
     lcd.print(i,BIN);
    
    // delay(10);
    
  }
  
}


// Assume 16 character lcd

void Characters(){
  
int  chartoprint=48;
char a;

        lcd.clear();
 
  for(int i=0 ; i < g_rows ; i++){
 

      for(int j=0 ; j < g_cols ; j++){
        
          lcd.setCursor(i,j);
          a = char(chartoprint);
          lcd.print(char(chartoprint));
          chartoprint++;
          if(chartoprint == 127)
              return;
 
      }


  }
 
 delay(1000);
}


void Fancy_Clear(){
  
     for (int i=0 ; i < g_rows ; i++){
       
       for(int j=0 ; j < g_cols/2 ; j++){
       lcd.setCursor(i,j);
       lcd.print(" ");
  
       lcd.setCursor(i, g_cols - j);
       lcd.print(" ");
       }
       //delay(10);
       
     }
    
}

void Every_Line(int lines){

  lcd.clear();
  for(int i=0 ; i < lines ; i++){
      lcd.setCursor(i,0);
      lcd.print("Line : ");
      lcd.print(i,DEC);
      
  }  
  delay(1000);
}

void Every_Pos(int lines,int cols){
  
  lcd.clear();

  for(int i=0 ; i < lines ; i++){
    for(int j=0 ; j< cols ; j++){
      lcd.setCursor(i,j);
      lcd.print(i,DEC);
    }

  } 
      delay(1000);  
}

void version(){

  lcd.command(0x06);
  delay(1000);
}

void BackLight(uint8_t bright){
  
  if(bright > 150)
      bright = 150;
      
  Wire.beginTransmission(0x4C);
  Wire.send(0xFE);
  Wire.send(0x03);
  Wire.send(bright);
  Wire.endTransmission();
  delay(50);
  
}

//
//  Default 15
//

void Contrast(uint8_t SetTo)
{
  
  if(SetTo > 100){
      SetTo = 100;
  }
  
  Wire.beginTransmission(0x4C);
  Wire.send(0xFE);
  Wire.send(0x04);
  Wire.send(SetTo);
  Wire.endTransmission();
  delay(50);
  
}
