#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// shiftout 

/* How many shift register chips are daisy-chained.
*/
#define NUMBER_OF_SHIFT_CHIPS   4

/* Width of data (how many ext lines).
*/
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8

/* Width of pulse to trigger the shift register to read and latch.
*/
#define PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
*/
#define POLL_DELAY_MSEC   1



int ploadPin        = 26;  // Connects to Parallel load pin the 165
int dataPin         = 22; // Connects to the Q7 pin the 165
int clockPin        = 24; // Connects to the Clock pin the 165

unsigned int pinValues_1;


/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
*/
unsigned int read_shift_regs()
{
    byte bitVal;
    unsigned int bytesVal = 0;

    /* Trigger a parallel Load to latch the state of the data lines,
    */
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N.
    */
    for(int i = 0; i < DATA_WIDTH; i++)
    {
     
     
        bitVal = digitalRead(dataPin);
        
  
        
        if (i == 0){ lcd.setCursor(0,0); }
      if (i == 8){ lcd.setCursor(0,1); } 
      if (i == 16){ lcd.setCursor(0,2);}
      if (i == 24){ lcd.setCursor(0,3);}
    lcd.print(bitVal);
        /* Set the corresponding bit in bytesVal.
        */
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        /* Pulse the Clock (rising edge shifts the next bit).
        */
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
        
      
     
    }

lcd.setCursor(10,1);
lcd.print(bytesVal);
lcd.setCursor(10,2);
lcd.print(~bytesVal);

    return(bytesVal);
}


void setup()
{
  
  
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  
  lcd.print("keypad test");

    /* Initialize our digital pins...
    */
    pinMode(ploadPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);
delay(1500);
}

void loop()
{
  lcd.clear();
    
     
      pinValues_1 = read_shift_regs();
 
     
   delay(500);
   

  
}

