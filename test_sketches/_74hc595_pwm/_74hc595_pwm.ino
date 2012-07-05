/**************************************************************
 * Name    : 74HC595 PWM SPI
 * By      : Joseph Francis
 *
 * Open Source
 *
 * Date    : 04 Jul, 2010                                      
 * Version : 1.3                                              
 * Notes   : Software PWM using daisy chained 74HC595 shift registers via SPI
 *
 * I wrote this code in a way that uses a srPins array to hold the byte pwm value for each pin.
 * Similar to doing an analogWrite, you can do a pwmWrite call to a "virtual" SR pin or just set srPins[x].
 * A timer process runs in the background to do PWM control, so the loop can concentrate on the process.
 *
 * Using this technique I have pwm controlled over 40 rgb leds with plenty of room for patterns, etc.
 *
 * While I would suggest using another chip to drive LEDs, the 595s provide an economical way to get started.
 ****************************************************************/

#include <TimerOne.h>

//--- Update TICKER_PWM_LEVELS to control how many levels you want
//     usually 32 levels of brightess is good (32K colors)
#define TICKER_PWM_LEVELS 32
#define TICKER_STEP 256/TICKER_PWM_LEVELS

//--- Update TIMER_DELAY to control how fast this runs.  
//    As long as you don't see flicker ... a higher number (slower) is better to assure
//    the loop has room to do stuff
#define TIMER_DELAY 280
#define SR_COUNT 6

//--- Used in iProcess to control the software PWM cycle
int ticker = 0;

//--- Pin connected to ST_CP of 74HC595
int latchPin = 10;
//--- Pin connected to SH_CP of 74HC595
int clockPin = 13;
//--- Pin connected to DS of 74HC595
int dataPin = 11;

//--- Used for faster latching
int latchPinPORTB = latchPin - 8;

//--- Holds a 0 to 255 PWM value used to set the value of each SR pin
byte srPins[SR_COUNT*8];

//--- Function used to set a shift register pin with a PWM value
//    can also just set srPins .. using this function allows you to map to pins if you need
//    a virtual mapping
void pwmWrite(int port, byte val){
  srPins[port] = val;  
}

//--- This process is run by the timer and does the PWM control
void iProcess(){
  //--- Create a temporary array of bytes to hold shift register values in
  byte srVals[SR_COUNT];
  //--- increment our ticker
  ticker++;
  //--- if our ticker level cycles, restart
  if( ticker > TICKER_PWM_LEVELS )
    ticker = 0;
  //--- get ticker as a 0 to 255 value, so we can always use the same data regardless of actual PWM levels
  int myPos = ticker * TICKER_STEP;

  //--- Loop through all bits in the shift register (8 pin for the 595's)
  for (int i = 0 ; i < 8; i++ ){
    int myLev = 0;
    //--- Loop through all shift registers and set the bit on or off
    for (int iSR = 0 ; iSR < SR_COUNT ; iSR++){
      //--- Start with the bit off
      myLev = 0;
      //--- If the value in the sr pin related to this SR/Byte is over the current pwm value
      //     then turn the bit on
      if (srPins[i+(iSR*8)] > myPos)
        myLev = 1;
      //--- Write the bit into the SR byte array
      bitWrite(srVals[iSR],i,myLev );
    }

  }


  //--- Run through all the temporary shift register values and send them (last one first)
  // latching in the process
  latchOff();
  for (int iSR = SR_COUNT-1 ; iSR >= 0 ; iSR--){
    spi_transfer(srVals[iSR]);
  }
  latchOn();

}


//--- Direct port access latching
void latchOn(){
  bitSet(PORTB,latchPinPORTB);
}
void latchOff(){
  bitClear(PORTB,latchPinPORTB);
}

//--- Used to setup SPI based on current pin setup
//    this is called in the setup routine;
void setupSPI(){
  byte clr;
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  SPCR &= ~( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  clr=SPSR; // clear SPI status reg
  clr=SPDR; // clear SPI data reg
  SPSR |= (1<<SPI2X); // set prescaler bits
  delay(10);
}


//--- The really fast SPI version of shiftOut
byte spi_transfer(byte data)
{
  SPDR = data;			  // Start the transmission
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR;			  // return the received byte, we don't need that
}

//---- ignore (used for multiplexing but disabled for example)
#define GROUND_1 4
#define GROUND_2 5
#define GROUND_3 6
#define GROUND_4 7
//-- end ig

void setup() {
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  digitalWrite(latchPin,LOW);
  digitalWrite(dataPin,LOW);
  digitalWrite(clockPin,LOW);
  
  //used for multiplexing
  pinMode(GROUND_1, OUTPUT);
  pinMode(GROUND_2, OUTPUT);
  pinMode(GROUND_3, OUTPUT);
  pinMode(GROUND_4, OUTPUT);
  
  digitalWrite(GROUND_1,HIGH);
  digitalWrite(GROUND_2,HIGH);
  digitalWrite(GROUND_3,HIGH);
  digitalWrite(GROUND_4,HIGH);
  
  //--- Setup to run SPI
  setupSPI();

  //--- Activate the PWM timer
  Timer1.initialize(TIMER_DELAY); // Timer for updating pwm pins
  Timer1.attachInterrupt(iProcess);

}

//--- Sets all the pins to the PWM value passed
void allTo(byte theVal){
  for( int i = 0 ; i < (SR_COUNT*8) ; i++ ){
    srPins[i] = theVal;
  }
}

//--- Blinks all pins full on and then off.  Total time for blink passed as the duration
void allBlink(int theDuration){
  for( int i = 0 ; i < (SR_COUNT*8) ; i++ ){
    srPins[i] = 255;
    delay(theDuration/2);
    srPins[i] = 0;
    delay(theDuration/2);
  }
}



//--- Very simple demo loop
void loop(){

 allBlink(100);

}
 

