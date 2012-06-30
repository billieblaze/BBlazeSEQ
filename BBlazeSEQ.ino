/*BBlazeSeq v0.8 */

// TODO: redo bottom button column connector! 
// TODO: create / save(pattern knob button) multiple patterns - calculate out the storage requirements for such data and how many patterns i could hold 
// TODO:  integrate control knobs and rework the ui for modifying params
// TODO: record mode 
// TODO: allow CC data in the pattern - maybe just filter initially? 

#include "constants.h"
#include <Wire.h>
#include "SPI.h"
#include "hsv2rgb.h"
#include <TimerThree.h>
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include <ShiftMatrixPWM.h> // included last because pins are declared in config.h


// Default Song Data
boolean patternData[][5][32]  =  
{
  {
    {      
      1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0                } 
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

// Setup
void setup(){

  // Setup UI 
  lcd.init();                   
  lcd.backlight();
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");

  setupLEDMatrix(); 
  setupKeypad();

  // Setup Midi events
  MIDI.begin();
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleStart);
  MIDI.turnThruOff();  

  // Setup Complete
  lcd.print("DONE");
  delay(50);
  lcd.clear();

  // start the keypad scanning
  Timer3.initialize(300000); 
  Timer3.attachInterrupt( handleKeypad ); 
}


// MIDI Callbacks
void HandleStart(){
  MIDIClockCounter = 0;
  tickCounter=0;
  runSequencer = 1;

  // send patch numbers
  for(int channel=0; channel <= channels; channel++){
    MIDI.sendProgramChange(patternData[channel][3][0], channel+1);
  }

}

void HandleStop(){  
  runSequencer = 0; 
}

void HandleClock(){

  if(  runSequencer == 1){

    // play notes every time the clock divider rolls over
    if (MIDIClockCounter == 0){
      playNotes();
      tickCounter++;
    } 

    MIDIClockCounter++;

    // bar complete - start over  
    if(tickCounter == 32){ 
      tickCounter=0;
    } 

    // reset the click divider
    if (MIDIClockCounter == 3){ 
      MIDIClockCounter=0; 
    } 
  }

}

void playNotes(){
  for(int channel=0; channel <= channels; channel++){
    int noteOn = patternData[channel][0][tickCounter];
    int note = patternData[channel][1][tickCounter];
    int velocity = patternData[channel][2][tickCounter];

    if (noteOn == 0 ){ 
      MIDI.sendNoteOff( lastNote[channel],0,channel+1);
    }

    if ( noteOn == 1 ) { 
      MIDI.sendNoteOn( note, velocity,channel+1);
      lastNote[channel] = note;
    }
    
    // hold the note plz   
    if (noteOn == 2) {  
    }

    
  } 
}


// Main Loop
void loop(){

  MIDI.read();
  handleMatrix();

  if (updateLCD == 1){
    handleLCD();
    updateLCD = 0;
  }

}

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

void handleLCD(){
  lcd.setCursor(0,0);
  lcd.print("CH");
  lcd.print(currentChannel+1);
  lcd.print(" - Step ");
  lcd.print(currentStep);
  lcd.print(" ");

  if (editMode == 0){ 
    lcd.print("PLAY");  
  } 

  if (editMode == 1){ 
    lcd.print("EDIT");  
  } 

  if (editMode == 2){ 
    lcd.print("REC ");  
  } 

  lcd.setCursor(0,1);
  lcd.print("ON:");
  if(editParam == 0){
    lcd.print(">");
  }


  int noteOnData = patternData[currentChannel][0][currentStep];
  switch(noteOnData){ 
  case 0: 
    lcd.print("off");
    break;
  case 1: 
    lcd.print("on");
    break;
  case 2: 
    lcd.print("hld");
    break;

  }

  lcd.print(" N#:");
  if(editParam == 1){
    lcd.print(">");
  }
  lcd.print(patternData[currentChannel][1][currentStep]);

  lcd.print(" V:");
  if(editParam == 2){
    lcd.print(">");
  }
  lcd.print(patternData[currentChannel][2][currentStep]);
  lcd.print(" ");

  lcd.setCursor(0,2);

  if(editParam == 3){
    lcd.print(">");
  }
  lcd.print("Patch ");
  lcd.print(patternData[currentChannel][3][0]);
}

void handleKeypad(){
  
  // scan the key matrix
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  254);
  read_shift_regs(1);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  247);
  read_shift_regs(2);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  251);
  read_shift_regs(3);
  shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  253);
  read_shift_regs(4);
  
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

/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
 */
unsigned int read_shift_regs(int row)
{
  byte bitVal;


  /* Trigger a parallel Load to latch the state of the data lines,*/
  digitalWrite(ploadPin, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);
  digitalWrite(ploadPin, HIGH);

  /* Loop to read each bit value from the serial out line     * of the SN74HC165N.    */

  for(int i = 1; i <= DATA_WIDTH; i++)
  {
    bitVal = digitalRead(dataPin);
    curPosition = buttonMapping[row-1][i-1];

  // update if something was released
   if (bitVal == 1 && editMode == 2 &&  recordLastNote == curPosition && recordLastPosition == i) {     
       MIDI.sendNoteOff(recordLastNote+oct3,0,currentChannel +1);  
       patternData[currentChannel][0][tickCounter] = 0;
       patternData[currentChannel][1][tickCounter] = curPosition+oct3;
       recordLastNote=0;
       recordLastPosition = 0;
   }
 
    // update the pattern if something was pressed! 
 
    if(bitVal == 0){
      // Play Mode
      if (editMode == 0){
        if ( patternData[currentChannel][0][curPosition] == 1){ 
          patternData[currentChannel][0][curPosition] = 0;

        } 
        else { 
          patternData[currentChannel][0][curPosition] = 1;
        }
      }
      
      // Edit Mode
      if (editMode == 1) {
        currentStep = curPosition;
        updateLCD=1;  
      }
      
      // Record Mode
      if (editMode == 2) {     
        if (recordLastNote == curPosition){
             // hold it
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
          
          
      } 
    }
    
    /* Pulse the Clock (rising edge shifts the next bit).        */
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(clockPin, LOW);
  }
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

