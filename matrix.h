
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
  ShiftMatrixPWM.SetAll(0);
  delay(200);
  ShiftMatrixPWM.SetAll(255);
  delay(200);
  
  ShiftMatrixPWM.SetAll(0);
}

void handleMatrix(){
  int i= 0;
if (  updateMatrix == 1){
  // loop thru the grid
  for(int row=0;row<numRows;row++){
    for(int col=0;col<(numColumns/3);col++){
      int RED = 0;
      int GREEN = 0;
      int BLUE = 0;
         
     // selected notes  
      int currentNote = patternData[currentChannel][0][i];
       
      if (currentNote == 1) {     
        if( i == 1 || i % 2 == 0){ 
            BLUE = 10;
        } else { 
            RED = 10;
            BLUE = 10;
        }
      }
      
      if (currentNote == 2){   // hold the note 
          RED = 10;
          BLUE = 5;
      }
       
      // current song positon
      if ( i == tickCounter){
        lastRow = row;
        lastCol = col;   
        setGroupOf3(lastRow, lastCol*3, 0,0,0);  
        RED = 10;
        BLUE = 10;
      }

      setGroupOf3(row, col*3, RED, GREEN, BLUE);
     
      i++;  
    }
  }
   updateMatrix = 0;
}
}
