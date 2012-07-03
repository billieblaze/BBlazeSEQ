String version = "0.9";

int initialized = 0;

// Init shiftMatrixPWM LED Matrix

// ShiftMatrixPWM connects to a group of 3 daisy chained 74hc595's to control RGB (columns) and a seperate 74hc595 (rows)
  //Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
  //Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
    const int ShiftMatrixPWM_columnLatchPin=49;
    const int ShiftMatrixPWM_rowDataPin=A15;
    const int ShiftMatrixPWM_rowClockPin=A14;
    const int ShiftMatrixPWM_rowLatchPin=A13;
    
    const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.
    const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.
    unsigned char maxBrightness = 0;
    unsigned char pwmFrequency = 50;
    
    int numColumnRegisters = 3;
    int numRows=4;
    
    int numColumns = numColumnRegisters*8;
    int numOutputs = numColumns*numRows;
    
    int lastRow = 0;
    int lastCol = 0;

    // button -> tick mapping
    boolean buttonMapping[4][8] = 
    {
      {31,30,29,28,27,24,25,26},
      {7,6,5,4,3,0,1,2},
      {15,14,13,12,11,8,9,10},
      {23,22,21,20,19,16,17,18},
    };


// LCD - using a sainSmart i2c LCD connected to 20/21 of my MEGA
    LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// keypad 
// The keypad connects to 74hc164 / 165 to shift in / out keys 
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
    
    // shiftout
    
    int ploadPin        = A11;  // Connects to Parallel load pin the 165
    int dataPin         = A10; // Connects to the Q7 pin the 165
    int clockPin        = A12; // Connects to the Clock pin the 165
    
    byte oldKeyPadValues[]={255,255,255,255};
    byte newKeyPadValues[]={255,255,255,255};


// navigation keys 
int navploadPin        = 26;  // Connects to Parallel load pin the 165
int navdataPin         = 22; // Connects to the Q7 pin the 165
int navclockPin        = 24; // Connects to the Clock pin the 165

byte navKeyState;

// Clock and Counter - all the bits the sequencer uses to track position / time / etc
    int channels = 2;
    int currentChannel = 0;  // which channel are we viewing?
    int currentStep = 0;
    int MIDIClockCounter = 0;
    int tickCounter = 0;
    int runSequencer=0;
    int editMode=0;
    int editParam=0;
    int updateLCD = 0;

    int lastNote[4] = {0,0,0,0};
    int recordLastNote = 0;
    int recordLastPosition = 0;
    int curPosition = 0;
    int keyOctave = 3;
    
    // Default Song Data
boolean patternData[][5][32]  =  
{
  {
    {      
      1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0                } 
    ,        // Note on / off
    {      
      C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3                }    
    ,    // Note Number
    {      
      127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127               }
    ,  // Velocity  
    {   10     } // config  - first is patch number
  }
  ,
  {
    {      
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                } 
    ,        // Note on / off
    {      
      C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3                }    
    ,    // Note Number
    {      
      127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127               }
    ,  // Velocity  
    {   
      0     } // config  
  }
  ,
  {
    {      
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                } 
    ,        // Note on / off
    {      
      C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3                }    
    ,    // Note Number
    {      
      127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127               }
    ,  // Velocity  
    {   
      0     } // config  
  }
  ,
  {
    {      
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                } 
    ,        // Note on / off
    {      
      C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,  C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3                }    
    ,    // Note Number
    {      
      127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127               }
    ,  // Velocity  
    {  
      0      } // config  
  }
  ,  
};

