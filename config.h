String version = "0.6";

// Init shiftMatrixPWM LED Matrix
  //Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
  //Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
  const int ShiftMatrixPWM_columnLatchPin=3;
  const int ShiftMatrixPWM_rowDataPin=4;
  const int ShiftMatrixPWM_rowClockPin=5;
  const int ShiftMatrixPWM_rowLatchPin=6;
  
  const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.
  const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.
   unsigned char maxBrightness = 63;
unsigned char pwmFrequency = 60;

int numColumnRegisters = 3;
int numRows=4;

int numColumns = numColumnRegisters*8;
int numOutputs = numColumns*numRows;

int lastRow = 0;
int lastCol = 0;

// LCD Keys
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

DFR_Key keypad;
int localKey = 0; 
String keyString = "";

// keypad 

// shiftout 
const int keypadOutputClockPin=A5;
const int keypadOutputDataPin=A4;

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



int ploadPin        = A1;  // Connects to Parallel load pin the 165
int dataPin         = A3; // Connects to the Q7 pin the 165
int clockPin        = A2; // Connects to the Clock pin the 165

unsigned int pinValues_1;
unsigned int pinValues_2;
unsigned int pinValues_3;
unsigned int pinValues_4;
unsigned int oldPinValues_1;
unsigned int oldPinValues_2;
unsigned int oldPinValues_3;
unsigned int oldPinValues_4;

// Clock and Counter
int channels = 4;
int currentChannel = 0;  // which channel are we viewing?

int MIDIClockCounter = 0;
int tickCounter = 0;

int runSequencer=0;


