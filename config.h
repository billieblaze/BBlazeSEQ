String version = "0.9";
// ShiftMatrixPWM handles the led matrix and connects to a group of 3 daisy chained 74hc595's to control RGB (columns) and a seperate 74hc595 (rows)
  //Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
  //Clock pin is SCK (atmega168/328: pin 13. Mega: 52)

    const int ShiftMatrixPWM_columnLatchPin=49;
    const int ShiftMatrixPWM_rowDataPin=A15;
    const int ShiftMatrixPWM_rowClockPin=A14;
    const int ShiftMatrixPWM_rowLatchPin=A13;
    
    const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.
    const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.
    
    
// The key matrix connect to 74hc164 / 165 to shift in / out keys 
    const int keypadOutputClockPin=A9;
    const int keypadOutputDataPin=A8;
    
    // shiftout
    int ploadPin        = A11;  // Connects to Parallel load pin the 165
    int dataPin         = A10; // Connects to the Q7 pin the 165
    int clockPin        = A12; // Connects to the Clock pin the 165
    


// navigation keys 
    int navploadPin        = 26;  // Connects to Parallel load pin the 165
    int navdataPin         = 22; // Connects to the Q7 pin the 165
    int navclockPin        = 24; // Connects to the Clock pin the 165
    
    
    
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

    
// Clock and Counter - all the bits the sequencer uses to track position / time / etc
    int channels = 15;
    int gate[] = { 41,39,37, 35, 33, 31,29,27 }; 
 
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


};
