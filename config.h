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
    
    unsigned char maxBrightness = 1;
    unsigned char pwmFrequency = 60;
    
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

  
    int encoderMapping[9][3] = {
    // byte array row, then the actual bit (0,1) number position for that encoder
      {0,2,3}, // ENC 1
      {1,4,5}, // ENC 2
      {2,7,6}, // ENC 3
      {2,2,3}, // ENC 4
      {0,5,6}, // ENC 5
      {0,0,1}, // ENC 6
      {1,1,2}, // ENC 7
      {2,3,4}, // ENC 8
      {3,1,2}, // ENC 9  - pattern knob
    };
    
    int encoderButtonMapping[9] = {
      0, // ENC_B 1
      9, // ENC_B 2
      15, // ENC_B 3
      23, // ENC_B 4
      8, // ENC_B 5
      0, // ENC_B 6
      12, // ENC_B 7
      20, // ENC_B 8
      31  // ENC_B 9
    };
    
    int navButtonMapping[8] = {
      27, // up
      25, // down
      28, // left
      26, // right
      25, // center
    };

// Clock and Counter - all the bits the sequencer uses to track position / time / etc
    int channels = 2;
          int updateMatrix = 1;
        int updateLCD = 1;
        
          byte oldNavKeyValues[]={255,255,255,255};
    byte newNavKeyValues[]={255,255,255,255};


   
    unsigned long UILastUpdated=0;
    unsigned long startTime = 0;

    
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
