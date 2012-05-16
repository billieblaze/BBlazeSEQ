/*BBlazeSeq v0.6 */

#include "constants.h"
#include <Wire.h> 
#include "SPI.h"
#include "hsv2rgb.h"
//#include <avr/pgmspace.h>
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include <DFR_Key.h>
#include "config.h"

// Default Song Data
boolean patternData[][3][32]  =  
{
  {
    {      1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0            } ,        // Note on / off
    {      C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3,C+oct3            }    ,    // Note Number
    {      127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127           }  // Velocity  }
  }
};
   unsigned int bytesVal[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
   
//shiftPWM
  #include <ShiftMatrixPWM.h>
 
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
 
   lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");

  keypad.setRate(1000);
  setupLEDMatrix();  
  setupKeypad();

  delay(250);

  lcd.print("DONE");
  delay(250);
  lcd.clear();
  MIDI.begin();
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleStart);

}


// MIDI Callbacks
void HandleStart(){
  MIDIClockCounter = 0;
  tickCounter=0;
  runSequencer = 1;
}

void HandleStop(){
  runSequencer = 0;
}

void HandleClock(){
  if(  runSequencer == 1){
    if (MIDIClockCounter == 0){
      playNotes();
      tickCounter++;
    } 
  
    MIDIClockCounter++;
      
    if(tickCounter == 32){ 
      tickCounter=0;
    } 
  
    if (MIDIClockCounter == 6){ 
      MIDIClockCounter=0; 
    }
  }
}

void playNotes(){
int channel = 0;
  //for(int channel=0; channel <= channels; channel++){
    int noteOn = patternData[channel][0][tickCounter];
    int note = patternData[channel][1][tickCounter];
    int velocity = patternData[channel][2][tickCounter];

     MIDI.sendNoteOff( note,0,channel+1);
    if ( noteOn == 1 ) { 
      MIDI.sendNoteOn( note, velocity,channel+1);
   }
  //} 
}


// Main Loop
void loop(){
  handleMIDI();
  handleLCD();
  handleKeypad();
  handleMatrix();
}


// Main Loop Handlers
void handleMIDI(){
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    switch(MIDI.getType()) {		// Get the type of the message we caught
    default:

      break;
    }
  } 
}

void handleLCD(){
  lcd.setCursor(0,0);
  if( runSequencer == 1){

    lcd.print("RUN");
    lcd.print("-");
    lcd.print(tickCounter);
    lcd.print("   ");
  } 
  else { 
    lcd.print("WAIT");
  }

}


void setupKeypad(){
pinMode(keypadOutputClockPin, OUTPUT); // make the clock pin an output
  pinMode(keypadOutputDataPin , OUTPUT); // make the data pin an output



    /* Initialize our digital pins...
    */
    pinMode(ploadPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);  
}

void handleKeypad(){
  
   shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  254);
   read_shift_regs(1);
   shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  253);
   read_shift_regs(2);
   shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  251);
   read_shift_regs(3);
   shiftOut(keypadOutputDataPin, keypadOutputClockPin, LSBFIRST,  247);
   read_shift_regs(4);
}

/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
*/
unsigned int read_shift_regs(int row)
{
    byte bitVal;
   
    int curPosition = 0;
    
    /* Trigger a parallel Load to latch the state of the data lines,*/
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);

    /* Loop to read each bit value from the serial out line     * of the SN74HC165N.    */

    for(int i = 1; i <= DATA_WIDTH; i++)
    {
        bitVal = digitalRead(dataPin);
        
        switch(row){
           case 1: 
               switch(i){ 
                 case 1:
                   curPosition = 7;
                    break;
                 case 2:
                   curPosition = 6;
                    break;
                 
                 case 3:
                   curPosition = 5;
                    break;
                 
                 case 4:
                   curPosition = 4;
                    break;
                 
                 case 5:
                   curPosition = 3;
                    break;
                 
                 case 6:
                   curPosition = 0;
                    break;
                 
                 case 7:
                   curPosition = 1;
                    break;
                 
                 case 8:
                   curPosition = 2;
                    break;
               }      
             break;
           
           case 2:
             switch(i){ 
                 case 1:
                   curPosition = 15;
                    break;
                 case 2:
                   curPosition = 14;
                    break;
                 
                 case 3:
                   curPosition = 13;
                    break;
                 
                 case 4:
                   curPosition = 12;
                    break;
                 
                 case 5:
                   curPosition = 11;
                    break;
                 
                 case 6:
                   curPosition = 8;
                    break;
                 
                 case 7:
                   curPosition = 9;
                    break;
                 
                 case 8:
                   curPosition = 10;
                    break;
               }      
           break;
           
           case 3:
           switch(i){ 
                 case 1:
                   curPosition = 23;
                    break;
                 case 2:
                   curPosition = 22;
                    break;
                 
                 case 3:
                   curPosition = 21;
                    break;
                 
                 case 4:
                   curPosition = 20;
                    break;
                 
                 case 5:
                   curPosition = 19;
                    break;
                 
                 case 6:
                   curPosition = 16;
                    break;
                 
                 case 7:
                   curPosition = 17;
                    break;
                 
                 case 8:
                   curPosition = 18;
                    break;
               }      
           break;
           
           case 4:
           switch(i){ 
                 case 1:
                   curPosition = 31;
                    break;
                 case 2:
                   curPosition = 30;
                    break;
                 
                 case 3:
                   curPosition = 29;
                    break;
                 
                 case 4:
                   curPosition =28;
                    break;
                 
                 case 5:
                   curPosition = 27;
                    break;
                 
                 case 6:
                   curPosition = 24;
                    break;
                 
                 case 7:
                   curPosition = 25;
                    break;
                 
                 case 8:
                   curPosition = 26;
                    break;
               }      
           break;
           
        }
    
     // update the pattern if something was pressed! 
        if(bitVal == 0){
         if ( patternData[currentChannel][0][curPosition] == 1){ 
            patternData[currentChannel][0][curPosition] = 0;

          } else { 
            patternData[currentChannel][0][curPosition] = 1;
          }
          delay(100);
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
  setGroupOf3(lastRow, lastCol*3, 0,0,0);
  
  // loop thru the grid
  for(int row=0;row<numRows;row++){
    for(int col=0;col<(numColumns/3);col++){
       // currently selected notes
       if (patternData[currentChannel][0][i] == 1) {     
          setGroupOf3(row, col*3, 255,255,255);
       } else { 
           setGroupOf3(row, col*3, 0,0,0);
       }
      
      // current song positon
       if ( i == tickCounter){
           lastRow = row;
           lastCol = col;   
          setGroupOf3(row, col*3, 255,0,255);
       }
       
       i++;
      
    }
  }
}
