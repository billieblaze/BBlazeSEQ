//todo: create some sort of classing for ui widgets (button, encoder, keypad, pot on aIN)
// 64 byte stack beyond task switch and interrupt needs
  byte bitVal;  
  byte bytesVal;

    byte oldDINValues[]={255,255,255,255};
    byte newDINValues[]={255,255,255,255};
    
void setupDIN(){
  // nav keys
  pinMode(navploadPin, OUTPUT);
  pinMode(navclockPin, OUTPUT);
  pinMode(navdataPin, INPUT);

  digitalWrite(navclockPin, LOW);
  digitalWrite(navploadPin, HIGH);   
}
    
void scanDIN(){

    digitalWriteFast(navploadPin, LOW);
    digitalWriteFast(navploadPin, HIGH);

    for ( int row = 0; row < 4; row++){
      bytesVal = 0;
      
      for(int i = 0; i < 8; i++){
        bitVal = PINA & ( 1 << PA0 );
        bytesVal |= (bitVal  << (7 - i));

     
        digitalWriteFast(navclockPin, HIGH);
        digitalWriteFast(navclockPin, LOW);
      } 
       //Serial.println(bytesVal); 
      newDINValues[row]=bytesVal;      
    }
    
    if (newDINValues[0] != oldDINValues[0] || newDINValues[1] != oldDINValues[1] || newDINValues[2] != oldDINValues[2] || newDINValues[3] != oldDINValues[3] ){
     //Serial.println("Sema");
      chBSemSignal(&semDIN);
    }       
}

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
 

    for ( int i = 0; i < 10; i++){

      int row = encoderMapping[i][0];
      int key1 = encoderMapping[i][1];
      int key2 = encoderMapping[i][2];

      byte value1 = (~newDINValues[row] >> key1) & B00000001;        
      byte value1_old = (~oldDINValues[row] >> key1) & B00000001;

      byte value2 = (~newDINValues[row] >> key2) & B00000001;
      byte value2_old = (~oldDINValues[row] >> key2) & B00000001;

     if (value1 != value1_old || value2 != value2_old){ 


      /*  if (value1_old == 0 && value1 == 0 &&  value2_old ==1 && value2 == 0 ) {
          encoderChange(i,0); 

        }*/
        if (value1_old == 0 && value1 == 1 &&  value2_old ==0 && value2 == 0 ) {
          encoderChange(i,0); 

        }

/* 
        if (value1_old == 1 && value1 == 0 &&  value2_old ==1 && value2 == 1 ) {
          encoderChange(i,0); 

        }*/

        if (value1_old == 1 && value1 == 1 &&  value2_old ==0 && value2 == 1 ) {
          encoderChange(i,0); 

        }
/* 
        if (value1_old == 0 && value1 == 0 &&  value2_old ==0 && value2 == 1 ) {
          encoderChange(i,1);

        }  */

        if (value1_old == 0 && value1 == 1 &&  value2_old ==1 && value2 == 1 ) { 
          encoderChange(i,1);

        }  
        /* 
        if (value1_old == 1 && value1 == 0 &&  value2_old ==0 && value2 == 0 ) { 
          encoderChange(i,1);

        }  
*/
        if (value1_old == 1 && value1 == 1 &&  value2_old ==1 && value2 == 0 ) { 
          encoderChange(i,1);

        }  
        
      }
    }
  
}



void handleNavButtons(){
 int bitVal = 0;

  for (int row = 0; row < 4;  row++){
    

      int i = 0;
      for (byte mask = B10000000; mask>0; mask >>= 1) {
        
        bitVal = 0;

        if (newDINValues[row] & mask){ 
          bitVal = 1;
          int key = (8*row) + i;
          switch(key){
            case 0:
              // enc sw 5
              break;
            case 3://enc sw 1
              break;
            case 8:
              break;
            case 9:// enc sw 2
              break;
            case 12: // enc sw 7
              break;
            case 15:// enc sw2
              break;
            case 18:// enc sw 8         
              break;
            case 19: 
              break;
            case 20:
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
            case 31: // select knob sw
              break;
            }
    
          }
          i++;
      }

    }
  
}

