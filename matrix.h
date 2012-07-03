
void setupLEDMatrix(){

  //Setup LED Matrix
  pinMode(ShiftMatrixPWM_columnLatchPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowDataPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowClockPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowLatchPin, OUTPUT); 

  SPI.setBitOrder(LSBFIRST);
  // SPI_CLOCK_DIV2 is only a tiny bit faster in sending out the last byte. 
  // SPI transfer and calculations overlap for the other bytes.
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 

  ShiftMatrixPWM.SetMatrixSize(numRows, numColumnRegisters);
  ShiftMatrixPWM.Start(pwmFrequency,maxBrightness);  
  ShiftMatrixPWM.SetAll(0);
  delay(200);
  ShiftMatrixPWM.SetAll(255);
  delay(200);
  ShiftMatrixPWM.SetAll(0);
}

void setGroupOf3(int row, int start, int r, int g, int b){
  ShiftMatrixPWM.SetOne(row, start, r);
  ShiftMatrixPWM.SetOne(row, start+1, g);
  ShiftMatrixPWM.SetOne(row, start+2, b);
}



void handleMatrix(){
  int i= 0;

  // clear the last position indicator 
  if ( runSequencer == 1){
    setGroupOf3(lastRow, lastCol*3, 0,0,0);
  }
  // loop thru the grid
  for(int row=0;row<numRows;row++){
    for(int col=0;col<(numColumns/3);col++){

      int currentNote = patternData[currentChannel][0][i];

      // currently selected notes
      if (currentNote == 0) { 
        setGroupOf3(row, col*3, 0,0,0); 
      }

      if (currentNote == 1) {     
        if( i == 1 || i % 2 == 0){ 
          setGroupOf3(row, col*3, 0,0,250);
        } 
        else { 
          setGroupOf3(row, col*3, 10,0,200);
        }
      }

      if (currentNote == 2){   // hold the note 
        setGroupOf3(row, col*3, 10,0,150);
      }

      // current song positon
      if ( i == tickCounter){
        lastRow = row;
        lastCol = col;   
        setGroupOf3(row, col*3, 127,0,127);
      }

      // current step being edited
      if( currentStep == i){
        lastRow = row;
        lastCol = col;   
        setGroupOf3(row, col*3, 127,127,127);
      }
      i++;  
    }
  }
}

