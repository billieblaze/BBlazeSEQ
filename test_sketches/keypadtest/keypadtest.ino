#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// shiftout 
const int keypadOutputClockPin=A9;
const int keypadOutputDataPin=A8;

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



int ploadPin        = A11;  // Connects to Parallel load pin the 165
int dataPin         = A10; // Connects to the Q7 pin the 165
int clockPin        = A12; // Connects to the Clock pin the 165

unsigned int pinValues_1;
unsigned int pinValues_2;
unsigned int pinValues_3;
unsigned int pinValues_4;
unsigned int oldPinValues_1;
unsigned int oldPinValues_2;
unsigned int oldPinValues_3;
unsigned int oldPinValues_4;

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

    return(bytesVal);
}


void setup()
{
  
  
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  
  lcd.print("keypad test");
 pinMode(keypadOutputClockPin, OUTPUT); // make the clock pin an output
  pinMode(keypadOutputDataPin , OUTPUT); // make the data pin an output



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
    
      shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  254);
      delay(10);
      pinValues_1 = read_shift_regs();
 

      shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  253);
      delay(10);
      pinValues_1 = read_shift_regs();
      
      lcd.setCursor(0,1);
      shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  251);
      delay(10);
      pinValues_1 = read_shift_regs();

      shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  247);
      delay(10);
      pinValues_1 = read_shift_regs();
      
      
   delay(500);
   

  
}

