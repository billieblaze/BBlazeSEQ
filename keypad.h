    byte oldKeyPadValues[]={0,0,0,0};
    byte newKeyPadValues[]={0,0,0,0};



void setupKeypad(){
  // matrix keypad
  pinMode(keypadOutputClockPin, OUTPUT); // make the clock pin an output
  pinMode(keypadOutputDataPin , OUTPUT); // make the data pin an output

  pinMode(ploadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  digitalWrite(clockPin, LOW);
  digitalWrite(ploadPin, HIGH);  

  // nav keys
  pinMode(navploadPin, OUTPUT);
  pinMode(navclockPin, OUTPUT);
  pinMode(navdataPin, INPUT);

  digitalWrite(navclockPin, LOW);
  digitalWrite(navploadPin, HIGH);  
}

byte read_shift_regs(int row){
  byte bitVal;
  byte bytesVal = 0;

 
  digitalWriteFast(ploadPin, HIGH);
  

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N.
    */ 

    for(int i = 0; i < 8; i++)
    { 
        bitVal = digitalReadFast(dataPin);  
        bytesVal |= (bitVal << i);

        digitalWriteFast(clockPin, HIGH);
        digitalWriteFast(clockPin, LOW);
    }

    digitalWriteFast(ploadPin, LOW);
    return(bytesVal);
    
}

void scanKeypad(){

  // scan the key matrix
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  254);
  newKeyPadValues[0] = read_shift_regs(1);
  
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  247);
  newKeyPadValues[1] = read_shift_regs(2);
  
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  251);
  newKeyPadValues[2] = read_shift_regs(3);
  
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  253);
  newKeyPadValues[3] = read_shift_regs(4);
}

void handleKeypad(){
 
  for (int row = 0; row < 4;  row++){
    if (newKeyPadValues[row] != oldKeyPadValues[row]){

      oldKeyPadValues[row] = newKeyPadValues[row];
       
      for (int i = 0; i < 8; i++) {
        if (~newKeyPadValues[row] & (B00000001 << i) ){ 
 
          curPosition = buttonMapping[row][i];
          
          switch(editMode){ 
          
              case 0: // play
               
                  currentStep = curPosition; 
          
                  if ( patternData[currentChannel][0][curPosition] == 1){ 
                    patternData[currentChannel][0][curPosition] = 0;
                  } else { 
                    patternData[currentChannel][0][curPosition] = 1;
                  }
                break;
                
                
              case 1: // edit
                  currentStep = curPosition; 
                  updateLCD=1; 
                break;
                
                
              case 2: // record
          
               // pressed button
                    if (recordLastNote == curPosition){ // hold it
                         patternData[currentChannel][0][tickCounter] = 2;
                         patternData[currentChannel][1][tickCounter] = curPosition+oct3;
                    }
                      
                    if (recordLastNote != curPosition && recordLastPosition != i){
                        //MIDI.sendNoteOff(recordLastNote+oct3,0,currentChannel +1);  
                        MIDI.sendNoteOn(curPosition+oct3,127,currentChannel +1);
                       
                        patternData[currentChannel][0][tickCounter] = 1;
                        patternData[currentChannel][1][tickCounter] = curPosition+oct3;
                        
                        recordLastNote = curPosition;
                        recordLastPosition = i;   
                    } 
             
                
                break;
      
            } 
        
          } else { 
    
               switch(editMode){ 
          
              case 0: // play
                break;
              case 1:
                break;
              case 2: 
                //NEED TO MOVE THIS - RELEASE NOTE IS NOT FIRING! 
                  if ( recordLastNote == curPosition && recordLastPosition == i) {     
                     MIDI.sendNoteOff(recordLastNote+oct3,0,currentChannel +1);  
                     patternData[currentChannel][0][tickCounter] = 0;
                     patternData[currentChannel][1][tickCounter] = curPosition+oct3;
                     recordLastNote=0;
                     recordLastPosition = 0;
                   }
                 break;
              }
          }
      } 
      updateMatrix = 1;
    }
  }  
  
}

