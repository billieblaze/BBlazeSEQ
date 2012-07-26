#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <TimerThree.h>

// shiftout 

/* How many shift register chips are daisy-chained.
 */
#define NUMBER_OF_SHIFT_CHIPS   1

/* Width of data (how many ext lines).
 */
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8

/* Width of pulse to trigger the shift register to read and latch.
 */
#define PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
 */
#define POLL_DELAY_MSEC   1


// navigation keys     
int navploadPin        = 26;  // Connects to Parallel load pin the 165
int navdataPin         = 22; // Connects to the Q7 pin the 165
int navclockPin        = 24; // Connects to the Clock pin the 165

byte oldNavKeyValues[]={
  255,255,255,255};
byte newNavKeyValues[]={
  255,255,255,255};

int encoderMapping[9][3] = {
  // byte array row, then the actual bit (0,1) number position for that encoder
  {
    0,2,3  }
  , // ENC 1
  {
    1,5,4  }
  , // ENC 2
  {
    2,7,6  }
  , // ENC 3
  {
    2,2,3  }
  , // ENC 4
  {
    0,5,6  }
  , // ENC 5
  {
    0,0,1  }
  , // ENC 6
  {
    1,1,2  }
  , // ENC 7
  {
    2,3,4  }
  , // ENC 8
  {
    3,1,2  }
  , // ENC 9  - pattern knob
};


void scanUI(){
   byte bitVal;
  byte bytesVal;

  digitalWrite(navploadPin, LOW);

  digitalWrite(navploadPin, HIGH);

  for ( int row = 0; row < 4; row++){
    bytesVal = 0;
    lcd.setCursor(0,row);
    for(int i = 0; i < 8; i++){
     
      bitVal = PINA & ( 1 << PA0 );
 
      bytesVal |= ( bitVal << (7 - i));

      digitalWrite(navclockPin, HIGH);

      digitalWrite(navclockPin, LOW);
           lcd.print(bitVal);
           
    } 

    newNavKeyValues[row]=bytesVal;
 
  }

}
void setup()
{



  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();

  lcd.print("encoder test");

  /* Initialize our digital pins...
   */
  pinMode(navploadPin, OUTPUT);
  pinMode(navclockPin, OUTPUT);
  pinMode(navdataPin, INPUT);

  digitalWrite(navclockPin, LOW);
  delay(5);
  digitalWrite(navploadPin, HIGH);
  
  delay(500);

  lcd.clear();

}

void loop()
{
 if (millis() %5 == 0){ 
  scanUI(); 
  
 }
  handleEncoders();

}

void encoderChange(int encoder, int direction){
 lcd.setCursor(9,3);

  lcd.print("E");
  lcd.print(encoder);
  lcd.print("-- D");
  lcd.print(direction);  
}

void handleEncoders(){

  for ( int i = 0; i < 10; i++){

    int row = encoderMapping[i][0];
    int key1 = encoderMapping[i][1];
    int key2 = encoderMapping[i][2];
       
    byte value1 = (~newNavKeyValues[row] >> key1) & B00000001;        
    byte value1_old = (~oldNavKeyValues[row] >> key1) & B00000001;
    byte value2 = (~newNavKeyValues[row] >> key2) & B00000001;
    byte value2_old = (~oldNavKeyValues[row] >> key2) & B00000001;

    oldNavKeyValues[row] =    newNavKeyValues[row];
    if (value1 != value1_old || value2 != value2_old){ 

  
       lcd.setCursor(9,0);
    lcd.print(row);
    lcd.print(" ");
    lcd.print(key1);
    lcd.print(" ");
    lcd.print(key2);
    lcd.setCursor(9,1);
    lcd.print(value1);
    lcd.print(value1_old);
    lcd.print("--");
    lcd.print(value2);
    lcd.print(value2_old);
    
     if (value1_old == 0 && value1 == 0 &&  value2_old ==1 && value2 == 0 ) {
      encoderChange(i,0); 
     break;
    }
    
     if (value1_old == 0 && value1 == 1 &&  value2_old ==0 && value2 == 0 ) {
      encoderChange(i,0); 
     break;
    }
 
    
     if (value1_old == 1 && value1 == 0 &&  value2_old ==1 && value2 == 1 ) {
      encoderChange(i,0); 
     break;
    }
    
     if (value1_old == 1 && value1 == 1 &&  value2_old ==0 && value2 == 1 ) {
      encoderChange(i,0); 
     break;
    }
    
    if (value1_old == 0 && value1 == 0 &&  value2_old ==0 && value2 == 1 ) {
      encoderChange(i,1);
     break;
    }  
    
     if (value1_old == 0 && value1 == 1 &&  value2_old ==1 && value2 == 1 ) { 
      encoderChange(i,1);
     break;
    }  
   if (value1_old == 1 && value1 == 0 &&  value2_old ==0 && value2 == 0 ) { 
      encoderChange(i,1);
     break;
    }  

      if (value1_old == 1 && value1 == 1 &&  value2_old ==1 && value2 == 0 ) { 
      encoderChange(i,1);
     break;
    }  
    }
  }
}
