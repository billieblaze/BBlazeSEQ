/*
BBlazeSeq v0.5
 */

#include "SPI.h"
#include "hsv2rgb.h"
#include <avr/pgmspace.h>
#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include "util.h"

#define  noteoff_size 32   // size of the noteoff arrays in memory
#define  mask         0x80 // used for drum patterns. b10000000

// MIDI notes
#define  C   0
#define  Cs  1
#define  D   2
#define  Ds  3
#define  E   4
#define  F   5
#define  Fs  6
#define  G   7
#define  Gs  8
#define  A   9
#define  As  10
#define  B   11

// Octaves
#define  oct0  0
#define  oct1  12
#define  oct2  24
#define  oct3  36
#define  oct4  48
#define  oct5  60
#define  oct6  72
#define  oct7  84





String version = "0.5";

// Init shiftMatrixPWM LED Matrix

//Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
//Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
const int ShiftMatrixPWM_columnLatchPin=3;
const int ShiftMatrixPWM_rowDataPin=A2;
const int ShiftMatrixPWM_rowClockPin=A3;
const int ShiftMatrixPWM_rowLatchPin=A4;

const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.
const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.

#include <ShiftMatrixPWM.h>   // include ShiftMatrixPWM.h after setting the pins!

unsigned char maxBrightness = 63;
unsigned char pwmFrequency = 60;
int numColumnRegisters =4;
int numRows=4;

int numColumns = numColumnRegisters*3;
int numOutputs = numColumns*numRows;

int j = 0;


// LCD Keys
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
DFR_Key keypad;
int localKey = 0; 
String keyString = "";

// keypad 
int sensorPin = A5;    // select the input pin for the potentiometer

int sensorValue = 0;  // variable to store the value coming from the sensor

static int KEY1_ARV = 19; 
static int KEY2_ARV = 41; 
static int KEY3_ARV = 61; 
static int KEY4_ARV = 1000; 
static int KEY5_ARV = 73; 
static int KEY6_ARV = 140; 
static int KEY7_ARV = 200; 
static int KEY8_ARV = 254; 
static int KEY9_ARV = 40; 
static int KEY10_ARV = 80; 
static int KEY11_ARV = 115; 
static int KEY12_ARV = 149; 
static int KEY13_ARV = 27; 
static int KEY14_ARV = 54; 
static int KEY15_ARV = 77; 
static int KEY16_ARV = 104; 

int _threshold = 3;
int _curInput = 0;

// Clock and Counter
int channels = 4;
int runMode = 0;   
int editMode = 0;
int bpm = 128;

int pause = (int) (60000 / bpm) / 16;
int position = 0;
int bar_counter = 0;
int tb = 1; // total bars
unsigned int tick_counter = 0;
int tpb = 16; // ticks per bar
int myposition = 0;

boolean pattern[][3][16]  =  
{
  {
    {
      1,1,1,0,1,0,0,0,1,0,1,0,1,0,0,0            }
    ,        // Note on / off
    {
      40,45,45,0,50,0,0,0,60,0,60,0,70,0,0,0            }
    ,    // Note Number
    {
      127,127,127,0,255,0,0,0,127,0,255,0,255,0,0,0            }  // Velocity
  }
  ,
  {
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
    ,        // Note on / off
    {
      40,0,0,0,50,0,0,0,60,0,0,0,70,0,0,0            }
    ,    // Note Number
    {
      127,0,0,0,255,0,0,0,127,0,0,0,255,0,0,0            }  // Velocity
  }
  ,
  {
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
    ,        // Note on / off
    {
      40,0,0,0,50,0,0,0,60,0,0,0,70,0,0,0            }
    ,    // Note Number
    {
      127,0,0,0,255,0,0,0,127,0,0,0,255,0,0,0            }  // Velocity
  }
  ,
  {
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
    ,        // Note on / off
    {
      40,0,0,0,50,0,0,0,60,0,0,0,70,0,0,0            }
    ,    // Note Number
    {
      127,0,0,0,255,0,0,0,127,0,0,0,255,0,0,0            }  // Velocity
  }
  ,
};

// variables for tracking what is being viewed / edited in run mode
int currentChannel = 0;  // which channel are we viewing?

// Menu 
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
//beneath is list of menu items needed to build the menu
MenuItem start = MenuItem("Run");
MenuItem editSeq = MenuItem("Edit");
MenuItem settings = MenuItem("Settings");
MenuItem Channel = MenuItem("MIDI Channel");
MenuItem BPM = MenuItem("BPM");

//this function builds the menu and connects the correct items together
void menuSetup()
{
  //add the file menu to the menu root
  menu.getRoot().add(start); 
  start.addAfter(editSeq); 
  editSeq.addAfter(settings); 
  //setup the settings menu item
  settings.addRight(Channel);
  //we want looping both up and down
  Channel.addBefore(BPM);
  Channel.addAfter(BPM);
  BPM.addAfter(Channel);
  //we want a left movement to pint to settings from anywhere
  Channel.addLeft(settings);
  BPM.addLeft(settings);
  settings.addAfter(start);
}

// Here all use events are handled - This is where you define a behaviour for a menu item
void menuUseEvent(MenuUseEvent used){  
  if (used.item == start){ 
    startRun(); 
  }
  if (used.item == editSeq){ 
    startEdit();  
  }
}

// Here we get a notification whenever the user changes the menu
void menuChangeEvent(MenuChangeEvent changed){
  lcd.clear();
  lcd.print(changed.to.getName());
}

//shiftPWM
void setGroupOf3(int row, int start, int r, int g, int b){
  ShiftMatrixPWM.SetOne(row, start, r);
  ShiftMatrixPWM.SetOne(row, start+1, g);
  ShiftMatrixPWM.SetOne(row, start+2, b);
}

void showActiveNotes(){ 
  int i, row, col;
  for( i=0; i < 16; i++ ) { 
    if (pattern[currentChannel][0][i] == 1) { 
      switch(i){  
      case 0: 
        row=0;
        col=3; 
        break;
      case 1: 
        row=1;
        col=3;
        break;
      case 2: 
        row=2;
        col=3;
        break;
      case 3: 
        row=3;
        col=3;
        break;

      case 4: 
        row=0;
        col=2;
        break;

      case 5: 
        row=1;
        col=2;
        break;

      case 6: 
        row=2;
        col=2;
        break;

      case 7: 
        row=3;
        col=2;
        break;

      case 8: 
        row=0;
        col=1;
        break;

      case 9: 
        row=1;
        col=1;
        break;

      case 10: 
        row=2;
        col=1;
        break;

      case 11: 
        row=3;
        col=1;
        break;

      case 12: 
        row=0;
        col=0;
        break;

      case 13: 
        row=1;
        col=0;
        break;

      case 14: 
        row=2;
        col=0;
        break;

      case 15: 
        row=3;
        col=0;
        break;

      }
      setGroupOf3(row, col*3, 255,0,255);

    }
  }

}

// SETUP                 
void setup() { 

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("bblazeSEQ v");
  lcd.print(version);
  delay(250);

  lcd.setCursor(0,1);
  lcd.print("Init...");

  // setup keys
  keypad.setRate(1000);

  //Setup LED Matrix
  pinMode(ShiftMatrixPWM_columnLatchPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowDataPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowClockPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowLatchPin, OUTPUT); 

  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 

  ShiftMatrixPWM.SetMatrixSize(numRows, numColumnRegisters);
  ShiftMatrixPWM.Start(pwmFrequency,maxBrightness);  
  ShiftMatrixPWM.SetAll(0);

  // Start Midi
  Serial.begin(31250);

  showActiveNotes();
  // Setup Menu
  menuSetup(); 

  lcd.print("DONE");
  delay(500);


  // first menu page
  menu.moveDown();   
}

void loop() { 

  // Keep track of time when loop started
  unsigned long start_time = millis();

  // Update current position in the bar, 0 -> 31
  if( runMode == 1){
    position = tick_counter % tpb;
  }


  // check Keypad
  int _curKey = checkPads();      

  if(_curKey != 0){
    if ( pattern[currentChannel][0][_curKey] ==  1){
      pattern[currentChannel][0][_curKey] = 0;   
    } 
    else  {
      pattern[currentChannel][0][_curKey] = 1;  
    }
    showActiveNotes();
  }

  // check the keyboard 
  localKey = keypad.getKey();

  if (localKey != SAMPLE_WAIT){

    if(runMode == 0 && editMode == 0){ 
      switch (localKey) {
      case 3: 
        lcd.print('up');
        menu.moveUp(); 
        break;
      case 4: 
        menu.moveDown(); 
        break;
      case 5: 
        menu.moveRight(); 
        break;
      case 2: 
        menu.moveLeft(); 
        break;
      case 1: 
        menu.use(); 
        break;
      }

    } 
    else if (runMode == 1 && editMode == 0){

      switch (localKey) {
      case 3: 
        previousChannel(); 
        break;                // up = prev channel
      case 4: 
        nextChannel(); 
        break;              // down = next channel
        // these will be to move to the next bar
        //case 5: menu.moveRight(); break;
        //case 2: menu.moveLeft(); break;
      case 1: 
        stopRun();  
        break;  // Select button = STOP
      }   
    } 
    else if ( runMode == 0 && editMode == 1){ 
      switch (localKey) {
      case 3: 
        previousChannel(); 
        break;                // up = prev channel
      case 4: 
        nextChannel(); 
        break;              // down = next channel
      case 5: 
        editNextStep(); 
        break;	
      case 2: 
        editPreviousStep(); 
        break;
      case 1: 
        stopEdit();  
        break;  // Select button = STOP       
      } 
    }

  }

  if (runMode == 0 && editMode == 1){ 
    lcd.print("!"); 
    printCurrentPosition(position);
    printCurrentChannel();



  } 
  else if (runMode == 1 && editMode == 0){  
    int row, col;
    switch(position){ 
    case 0: 
      row=0;
      col=3; 
      break;
    case 1: 
      row=1;
      col=3;
      break;
    case 2: 
      row=2;
      col=3;
      break;
    case 3: 
      row=3;
      col=3;
      break;

    case 4: 
      row=0;
      col=2;
      break;

    case 5: 
      row=1;
      col=2;
      break;

    case 6: 
      row=2;
      col=2;
      break;

    case 7: 
      row=3;
      col=2;
      break;

    case 8: 
      row=0;
      col=1;
      break;

    case 9: 
      row=1;
      col=1;
      break;

    case 10: 
      row=2;
      col=1;
      break;

    case 11: 
      row=3;
      col=1;
      break;

    case 12: 
      row=0;
      col=0;
      break;

    case 13: 
      row=1;
      col=0;
      break;

    case 14: 
      row=2;
      col=0;
      break;

    case 15: 
      row=3;
      col=0;
      break;

    }


    setGroupOf3(row,col*3,255,255,255);




    printCurrentPosition(position);
    printCurrentChannel();

    //Process MELODIES
    int i;

    for(i =0; i <= channels; i++){
      int noteOn = pattern[i][0][position];
      int note = pattern[i][1][position];
      int velocity = pattern[i][2][position];

      if ( noteOn == 1 ) { 
        noteon(i, note, velocity);
       }    else{ 
        noteoff(i, note);
      }
    }
    setGroupOf3(row, col*3, 0,0,0);

    showActiveNotes();
    //-----------------------------------------------------------
    // TIMEKEEPING
    //-----------------------------------------------------------
    tick_counter++;

    if (tick_counter % tpb == 0 && tick_counter != 0) // We have reached the end of the bar
    {
      bar_counter++;
      tick_counter = 0;  
      position = 0;


      if (bar_counter % tb == 0 &&  bar_counter != 0){
        bar_counter = 0;
      }  
    }
    // Pause before next tick.
    while (millis() < start_time + pause) {
    } 
  }


}


//-----------------------------------------------------------
// MIDI message sending functions
//-----------------------------------------------------------

void noteon(byte channel, byte note, byte velocity) 
{
  midimsg( 0x90 | channel, note, velocity);
}

void noteoff(byte channel, byte note) 
{
  midimsg( 0x80 | channel, note, 0);
}

void program_change(byte channel, byte progno) 
{
  midimsg( 0xC0 | channel, progno);
}

// Three-byte MIDI message
void midimsg(byte cmd, byte data1, byte data2) 
{
  Serial.write(byte(cmd));
  Serial.write(byte(data1));
  Serial.write(byte(data2));
}

// Two-byte MIDI message
void midimsg(byte cmd, byte data1) 
{
  Serial.write(byte(cmd));
  Serial.write(byte(data1));
}


// function for maneuvering the channels while running / editing

void printCurrentChannel(){
  lcd.setCursor(0,1);
  lcd.print("Ch");
  lcd.print(currentChannel+1);    
}
void printCurrentPosition(int myposition){
  lcd.setCursor(0,0);  
  lcd.print("Bar ");
  lcd.print(bar_counter);
  lcd.print(" Tick ");
  lcd.print(myposition);
  lcd.print("    ");
}

void previousChannel(){
  if (currentChannel != 0){ 
    currentChannel--; 
    printCurrentChannel();
    showActiveNotes();
  }
}

void nextChannel(){ 
  if (currentChannel <= channels){
    currentChannel++;  
    printCurrentChannel();
    showActiveNotes();
  }
}

void editPreviousStep(){
  if (position > 0){ 
    if ( position != 0 && bar_counter !=0){
      position--;
    } 
    else { 
      bar_counter--;
      position=16; 
    }       

    printCurrentPosition(position);
  }
}

void editNextStep(){ 

  position++;   
  printCurrentPosition(position);

  if ( position > tpb){ 
    position=0;
    bar_counter++;
  }
}

void startEdit(){  
  editMode = 1;
}
void startRun(){ 
  runMode = 1; 
}
void stopEdit(){  
  editMode = 0;
}
void stopRun(){ 
  runMode = 0; 
}

int checkPads(){

  // read the value from the sensor:
  _curInput = analogRead(sensorPin);

  if (_curInput > 0){ 

    int _curKey = 0;

    if (_curInput > KEY1_ARV - _threshold && _curInput < KEY1_ARV + _threshold ) _curKey = 1;
    else if (_curInput > KEY2_ARV - _threshold && _curInput < KEY2_ARV + _threshold ) _curKey = 2;
    else if (_curInput > KEY3_ARV - _threshold && _curInput < KEY3_ARV + _threshold ) _curKey = 3;
    else if (_curInput > KEY4_ARV - _threshold && _curInput < KEY4_ARV + _threshold ) _curKey = 4;
    else if (_curInput > KEY5_ARV - _threshold && _curInput < KEY5_ARV + _threshold ) _curKey = 5;
    else if (_curInput > KEY6_ARV - _threshold && _curInput < KEY6_ARV + _threshold ) _curKey = 6;
    else if (_curInput > KEY7_ARV - _threshold && _curInput < KEY7_ARV + _threshold ) _curKey = 7;
    else if (_curInput > KEY8_ARV - _threshold && _curInput < KEY8_ARV + _threshold ) _curKey = 8;
    else if (_curInput > KEY9_ARV - _threshold && _curInput < KEY9_ARV + _threshold ) _curKey = 9;
    else if (_curInput > KEY10_ARV - _threshold && _curInput < KEY10_ARV + _threshold ) _curKey = 10;
    else if (_curInput > KEY11_ARV - _threshold && _curInput < KEY11_ARV + _threshold ) _curKey = 11;
    else if (_curInput > KEY12_ARV - _threshold && _curInput < KEY12_ARV + _threshold ) _curKey = 12;
    else if (_curInput > KEY13_ARV - _threshold && _curInput < KEY13_ARV + _threshold ) _curKey = 13;
    else if (_curInput > KEY14_ARV - _threshold && _curInput < KEY14_ARV + _threshold ) _curKey = 14;
    else if (_curInput > KEY15_ARV - _threshold && _curInput < KEY15_ARV + _threshold ) _curKey = 15;
    else if (_curInput > KEY16_ARV - _threshold && _curInput < KEY16_ARV + _threshold ) _curKey = 16;

    return _curKey;
  }       
}



