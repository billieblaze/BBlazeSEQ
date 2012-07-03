void handleNavKeys(){
  
  // scan the nav keys
  digitalWrite(navploadPin, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);
  digitalWrite(navploadPin, HIGH);

  for(int i = 0; i < 32; i++){
    int navVal = digitalRead(navdataPin);
    if (navVal == 0){
  
      navKeyState = i;
      switch(i){
      case 0:
        // enc sw 5
        editParam = 4;
        break;
      case 1: 
        break;
      case 2: 
        break;

      case 3://enc sw 1
        editParam=0;

        break;
      case 4:
        patternData[currentChannel][0][currentStep]++;
        break;
      case 5:
        patternData[currentChannel][0][currentStep]--;
        break;
      case 6: // enc sw 6
        editParam=5;

        break;
      case 7:

        break;
      case 8:
        break;
      case 9:// enc sw 2
        editParam=1;
        break;
      case 10: 
        patternData[currentChannel][1][currentStep]++;
        break;
      case 11:
        patternData[currentChannel][1][currentStep]--;
        break;
      case 12: // enc sw 7
        editParam=6;
        break;
      case 13:
        break;
      case 14:
        break;
      case 15:// enc sw2
        editParam=2;
        break;
      case 16: 
        patternData[currentChannel][2][currentStep]++;
        break;
      case 17:
        patternData[currentChannel][2][currentStep]--;
        break;
      case 18:// enc sw 8
        editParam=7;
        break;
      case 19: 
        break;
      case 20:
        break;
      case 21:// enc sw 4
        editParam = 3;
        break;
      case 22: 
        patternData[currentChannel][3][currentStep]++;
        break;
      case 23:
        patternData[currentChannel][3][currentStep]--;
        break;
      case 24:// down
        if (editMode == 0 && currentChannel != channels){
          currentChannel++; 
        }

        if (editMode == 1 ){
          if ( editParam == 0 && patternData[currentChannel][editParam][currentStep] == 0){ 
          } 
          else { 
            switch(editParam){
            case 3:
              patternData[currentChannel][editParam][0]--;


              MIDI.sendProgramChange(patternData[currentChannel][editParam][0], currentChannel+1);
            default:

              patternData[currentChannel][editParam][currentStep]--;
              break;
            }


          }        
        }
        break;
      case 25:// Select
   
        if (editMode == 0){ // Edit Mode
          editMode = 1;
          break;
        } 
        
        if ( editMode == 1 ){ // Rec Mode
         editMode = 2; 
         break;
        }
        
        if ( editMode == 2){ // Play Mode
          editMode = 0;
          break;
        }

      case 26:// Right

        if (editMode == 0 && currentStep != 32){
          currentStep++; 
        }

        if (editMode == 1 && editParam != 3){
          editParam++;
        }

        break;
      case 27:// UP

        if (editMode == 0 && currentChannel != 0){
          currentChannel--; 
        }

        if (editMode == 1 ){

          if ( editParam == 0 && patternData[currentChannel][editParam][currentStep] == 2){ 
          } 
          else { 
            switch (editParam){
            case 3: 
              patternData[currentChannel][editParam][0]++;
              MIDI.sendProgramChange(patternData[currentChannel][editParam][0], currentChannel+1);
              break;

            default:
              patternData[currentChannel][editParam][currentStep]++;
              break;
            }
          }

        }

        break;
      case 28:// Left
        if (editMode == 0 && currentStep != 0){
          currentStep--; 
        }
        if (editMode == 1 && editParam != 0){
          editParam--;
        }
        break;
      case 29:
        break;
      case 30:
        break;
      case 31:
        break;
      case 32:
        break;   

      }
      updateLCD = 1;
    }

    digitalWrite(navclockPin, HIGH);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(navclockPin, LOW);
  }
}
