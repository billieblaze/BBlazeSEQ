void scanNavKeys(){
  byte bitVal;
  byte bytesVal;

  digitalWrite(navploadPin, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);
  digitalWrite(navploadPin, HIGH);

  for ( int row = 0; row < 4; row++){
    bytesVal = 0;
    
    for(int i = 0; i < 8; i++){
      bitVal = digitalRead(navdataPin);
      bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

      digitalWrite(navclockPin, HIGH);
      delayMicroseconds(PULSE_WIDTH_USEC);
      digitalWrite(navclockPin, LOW);
    } 

    newNavKeyValues[row]=bytesVal;
  }
}

void handleNavKeys(){
 int bitVal = 0;

  for (int row = 0; row < 4;  row++){
    if (newNavKeyValues[row] != oldNavKeyValues[row]){
      
      oldNavKeyValues[row] = newNavKeyValues[row] ;   
      int i = 0;
      for (byte mask = 10000000; mask>0; mask >>= 1) {
        
        bitVal = 0;
        if (~newNavKeyValues[row] & mask){ 
          bitVal = 1;
          int key = (8*row) + i;
          switch(key){
            case 0:
              // enc sw 5
              break;

            case 1: // Note On 
              if ( patternData[currentChannel][0][currentStep] != 2){
                patternData[currentChannel][0][currentStep]++;
              }
              break;

            case 2: // Note On
              if ( patternData[currentChannel][0][currentStep] != 0){
                patternData[currentChannel][0][currentStep]--;
              }
              break;
      
            case 3://enc sw 1
              break;

            case 4:
              if (currentChannel != channels){
                currentChannel++; 
              }
              
              break;

            case 5:
              if (currentChannel != 0){
                currentChannel--; 
              }
             
              break;

            case 6: // enc sw 6
              if(patternData[currentChannel][1][currentStep] != 255){
                patternData[currentChannel][1][currentStep]++;
              }
              break;

            case 7:
              if(patternData[currentChannel][1][currentStep] != 0){
                patternData[currentChannel][1][currentStep]--;
              }
              break;

            case 8:
              break;

            case 9:// enc sw 2
              break;

            case 10: 
              if(currentStep != 32){
                currentStep++;
              }
              break;

            case 11:
              if(currentStep != 0){
                currentStep--;
              }
              break;

            case 12: // enc sw 7
              break;

            case 13:
              if (patternData[currentChannel][2][currentStep] != 127){
                patternData[currentChannel][2][currentStep]++;
              }
              break;

            case 14:
              if (patternData[currentChannel][2][currentStep] != 0){
                patternData[currentChannel][2][currentStep]--;
              }
              
              break;

            case 15:// enc sw2
              
              break;

            case 16: 
              if (editMode != 2){ // Edit Mode
                editMode++;
              } 
              break;

            case 17:
              if (editMode != 0){ // Edit Mode
                editMode--;
              } 
              break;

            case 18:// enc sw 8         
              break;

            case 19: 
              break;

            case 20:
              break;

            case 21:// enc sw 4
              patternData[currentChannel][3][0]++;
              MIDI.sendProgramChange(patternData[currentChannel][3][0], currentChannel+1);
              break;

            case 22:       
              patternData[currentChannel][3][0]--;
              MIDI.sendProgramChange(patternData[currentChannel][3][0], currentChannel+1);
              break;

            case 23:     
              break;

            case 24:// down  
              break;

            case 25:// Select
              break;
             
            case 26:// Right
              break;

            case 27:// UP
              break;

            case 28:// Left
              break;

            case 29:// select knob
              break;

            case 30: // select knob
              break;

            case 31: // select knob sw
              break;
      
            }
            
            updateLCD = 1;  
          }
          i++;
      }
    }
  }  
}
