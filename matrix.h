unsigned char maxBrightness = 10;
unsigned char pwmFrequency = 60;

int numColumnRegisters = 3; 
int numRows=4;

int numColumns = numColumnRegisters*8;
int numOutputs = numColumns*numRows;

int lastRow = 0;
int lastCol = 0;
   
void setGroupOf3(int row, int start, int r, int g, int b){
  ShiftMatrixPWM.SetOne(row, start, r);
  ShiftMatrixPWM.SetOne(row, start+1, g);
  ShiftMatrixPWM.SetOne(row, start+2, b);
}

void setupLEDMatrix(){

  //Setup LED Matrix
  pinMode(ShiftMatrixPWM_columnLatchPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowDataPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowClockPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowLatchPin, OUTPUT); 

  SPI.setBitOrder(LSBFIRST);
  // SPI_CLOCK_DIV2 is only a tiny bit faster in sending out the last byte. 
  // SPI transfer and calculations overlap for the other bytes.
  SPI.setClockDivider(SPI_CLOCK_DIV2); 
  SPI.begin(); 

  ShiftMatrixPWM.SetMatrixSize(numRows, numColumnRegisters);
  ShiftMatrixPWM.Start(pwmFrequency,maxBrightness);  
  ShiftMatrixPWM.PrintInterruptLoad();
  ShiftMatrixPWM.SetAll(255);
  
  for(int i = 255; i > 0; i--){
    ShiftMatrixPWM.SetAll(i);
    delay(50);
  }

}

void handleLEDMatrix(){
  
  int i= 0;
  // loop thru the grid
  for(int row=0;row<numRows;row++){
    for(int col=0;col<(numColumns/3);col++){
      int RED = 0;
      int GREEN = 0;
      int BLUE = 0;
         
     // selected notes  
      int currentNote = patternData[currentChannel][0][i];
       
      if (currentNote == 1) {     
            RED = 0;
            GREEN = 3; 
            BLUE = 10;
      }
      
      if (currentNote == 2){   // hold the note 
          RED = 0;
          GREEN = 10; 
          BLUE = 10;
      }
       
      // current song positon
      if ( i == tickCounter){
        lastRow = row;
        lastCol = col;   
        setGroupOf3(lastRow, lastCol*3, 0,0,0);  
        RED = 10;
        GREEN = 0;
        BLUE = 0;
      }

      setGroupOf3(row, col*3, RED, GREEN, BLUE);
     
      i++;  
    }
  
  } 
 
}


static WORKING_AREA(waProcessMatrix, 64);

static msg_t ProcessMatrix(void *arg) {
  
   
  while (1) {
     scanKeyMatrix();
    // Wait for signal from thread 2.
   chBSemWait(&semDIN);
 
    Serial.println("ProcessMatrix");
     handleLEDMatrix(); 
   
  }
  return 0;  
}


