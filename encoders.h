
void encoderChange( int encoder,int value){

  switch(encoder){

  case 0: // current channel
    if ( value == 0 && currentChannel != 0){
      currentChannel--; 
    }

    if (value == 1 && currentChannel != channels){
      currentChannel++; 
    }
    break; 

  case 1:  // step
    if( value == 0 && currentStep != 0){
      currentStep--;
    }

    if(value == 1 && currentStep != 32){
      currentStep++;
    }
    break;

  case 2: // mode
    if ( value == 0 && editMode != 0){ // Edit Mode
      editMode--;
    } 
    if (  value == 1 && editMode != 2){ // Edit Mode
      editMode++;
    } 
    break;

  case 3: // Patch

    if ( value == 0 && patternData[currentChannel][3][0] != 0){
      patternData[currentChannel][3][0]--;
      MIDI.sendProgramChange(patternData[currentChannel][3][0], currentChannel+1);
    }

    if ( value == 1 && patternData[currentChannel][3][0] != 255){  
      patternData[currentChannel][3][0]++;
      MIDI.sendProgramChange(patternData[currentChannel][3][0], currentChannel+1);
    }
    break;

  case 4: // Note on / off 

    if ( value == 0 && patternData[currentChannel][0][currentStep] != 0){
      patternData[currentChannel][0][currentStep]--;
    }

    if (  value == 1 && patternData[currentChannel][0][currentStep] != 2){
      patternData[currentChannel][0][currentStep]++;
    }

    break;

  case 5: // Note Number
    if(  value == 0 && patternData[currentChannel][1][currentStep] != 0){
      patternData[currentChannel][1][currentStep]--;
    }

    if(  value == 1 && patternData[currentChannel][1][currentStep] != 255){
      patternData[currentChannel][1][currentStep]++;
    }
    break;

  case 6: // Velocity
    if (  value == 0 && patternData[currentChannel][2][currentStep] != 0){
      patternData[currentChannel][2][currentStep]--;
    }

    if (  value == 1 && patternData[currentChannel][2][currentStep] != 127){
      patternData[currentChannel][2][currentStep]++;
    }  
    break;

  } 
}

void handleEncoders(){
  if (newNavKeyValues != oldNavKeyValues){

    for ( int i = 0; i < 10; i++){

      int row = encoderMapping[i][0];
      int key1 = encoderMapping[i][1];
      int key2 = encoderMapping[i][2];

      byte value1 = (~newNavKeyValues[row] >> key1) & B00000001;        
      byte value1_old = (~oldNavKeyValues[row] >> key1) & B00000001;

      byte value2 = (~newNavKeyValues[row] >> key2) & B00000001;
      byte value2_old = (~oldNavKeyValues[row] >> key2) & B00000001;

     if (value1 != value1_old || value2 != value2_old){ 


        if (value1_old == 0 && value1 == 0 &&  value2_old ==1 && value2 == 0 ) {
          encoderChange(i,0); 

        }

        if (value1_old == 0 && value1 == 1 &&  value2_old ==0 && value2 == 0 ) {
          encoderChange(i,0); 

        }


        if (value1_old == 1 && value1 == 0 &&  value2_old ==1 && value2 == 1 ) {
          encoderChange(i,0); 

        }

        if (value1_old == 1 && value1 == 1 &&  value2_old ==0 && value2 == 1 ) {
          encoderChange(i,0); 

        }

        if (value1_old == 0 && value1 == 0 &&  value2_old ==0 && value2 == 1 ) {
          encoderChange(i,1);

        }  

        if (value1_old == 0 && value1 == 1 &&  value2_old ==1 && value2 == 1 ) { 
          encoderChange(i,1);

        }  
        if (value1_old == 1 && value1 == 0 &&  value2_old ==0 && value2 == 0 ) { 
          encoderChange(i,1);

        }  

        if (value1_old == 1 && value1 == 1 &&  value2_old ==1 && value2 == 0 ) { 
          encoderChange(i,1);

        }  
        oldNavKeyValues[row] = newNavKeyValues[row];
        updateLCD = 1;
      }

    }
  }
}

