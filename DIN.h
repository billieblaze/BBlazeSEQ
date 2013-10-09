//todo: create some sort of classing for ui widgets (button, encoder, keypad, pot on aIN)



    // button -> tick mapping
    boolean buttonMapping[4][8] = 
    {
      {31,30,29,28,27,24,25,26},
      {7,6,5,4,3,0,1,2},
      {15,14,13,12,11,8,9,10},
      {23,22,21,20,19,16,17,18},

    };




  
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



    byte oldKeyPadValues[]={255,255,255,255};
    byte newKeyPadValues[]={255,255,255,255};
    
    
void setupKeyMatrix(){
  // matrix keypad
  pinMode(keypadOutputClockPin, OUTPUT); // make the clock pin an output
  pinMode(keypadOutputDataPin , OUTPUT); // make the data pin an output

  pinMode(ploadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  digitalWrite(clockPin, LOW);
  digitalWrite(ploadPin, HIGH);  

 
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



void scanKeyMatrix(){

  // scan the key matrix
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  254);
  newKeyPadValues[0] = read_shift_regs(1);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  247);
  newKeyPadValues[1] = read_shift_regs(2);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  251);
  newKeyPadValues[2] = read_shift_regs(3);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  253);
  newKeyPadValues[3] = read_shift_regs(4);
  
  if (newKeyPadValues[0] != oldKeyPadValues[0] || newKeyPadValues[1] != oldKeyPadValues[1] || newKeyPadValues[2] != oldKeyPadValues[2] ||newKeyPadValues[3] != oldKeyPadValues[3] ){
    chBSemSignal(&semDIN);
  }   
}

void handleKeyMatrix(){
 
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
                break;
                
                
              case 2: // record
          
               // pressed button
                    if (recordLastNote == curPosition){ // hold it
                         
                         digitalWriteFast(gate[currentChannel], LOW);
                         
                         patternData[currentChannel][0][tickCounter] = 2;
                         patternData[currentChannel][1][tickCounter] = curPosition+oct3;

                    }
                      
                    if (recordLastNote != curPosition && recordLastPosition != i){
                        //MIDI.sendNoteOff(recordLastNote+oct3,0,currentChannel +1);  
                        MIDI.sendNoteOn(curPosition+oct3,127,currentChannel +1);
                        
                       digitalWriteFast(gate[currentChannel], HIGH);
                       
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
                     digitalWriteFast(gate[currentChannel], LOW);
 
                     patternData[currentChannel][0][tickCounter] = 0;
                     patternData[currentChannel][1][tickCounter] = curPosition+oct3;
                     recordLastNote=0;
                     recordLastPosition = 0;
                   }
                 break;
              }
          }
      } 
    }
  }  
  
}



// Setup the UI thread
static WORKING_AREA(waScanUI, 64);

static msg_t ScanUI(void *arg) {
  Serial.print("Setup UI");
 

    
  while (1) {
     Serial.println("Scan UI");
     scanDIN();
     scanKeyMatrix();
     chThdSleepMilliseconds(80); 

   
  }
  return 0;  
}

static WORKING_AREA(waProcessUI, 64);

static msg_t ProcessUI(void *arg) {
  
   
  while (1) {
    // Wait for signal from thread 2.
   chBSemWait(&semDIN);
 
    Serial.println("ProcessUI");

     handleNavButtons();  
     handleEncoders(); 
     handleKeyMatrix();
     
    // update lcd
     chBSemSignal(&semLCD);
   
  }
  return 0;  
}



