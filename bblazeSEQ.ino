/*
BBlazeSeq v0.3
*/

#include "SPI.h"
#include "hsv2rgb.h"
#include <avr/pgmspace.h>
#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <DFR_Key.h>
#include "util.h"
#include "song.h"
  
#define  noteoff_size 32   // size of the noteoff arrays in memory
#define  mask         0x80 // used for drum patterns. b10000000

String version = "0.3";

// LCD Keys
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
DFR_Key keypad;
int localKey = 0; 
String keyString = "";

// Clock and Counter
int runMode = 0;   
int editMode = 0;

int pause = (int) (60000 / bpm) / 8;
int position = 0;
int bar_counter = 0;
int tb = 15; // total bars
unsigned int tick_counter = 0;
int tpb = 16; // ticks per bar
int myposition = 0;

// variables for tracking what is being viewed / edited in run mode
int currentChannel = 0;  // which channel are we viewing?

// Noteoff arrays - Used to keep track of and trigger noteOff messages
int  noteoff_times[noteoff_size];
byte noteoff_notes[noteoff_size];
byte noteoff_chans[noteoff_size];


// shiftMatrixPWM LED Matrix

//Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
//Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
const int ShiftMatrixPWM_columnLatchPin=3;
const int ShiftMatrixPWM_rowDataPin=A1;
const int ShiftMatrixPWM_rowClockPin=A2;
const int ShiftMatrixPWM_rowLatchPin=A3;

const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.

const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.

#include <ShiftMatrixPWM.h>   // include ShiftMatrixPWM.h after setting the pins!

unsigned char maxBrightness = 63;
unsigned char pwmFrequency = 60;
int numColumnRegisters =3;
int numRows=4;

int numColumns = numColumnRegisters*4;
int numOutputs = numColumns*numRows;

int j = 0;

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
    if (used.item == start){ startRun(); }
    if (used.item == editSeq){ startEdit();  }
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

void active(){ 
setGroupOf3(0, 0, 155,12,120);
   setGroupOf3(1, 3, 155,12,120);
   setGroupOf3(1, 9, 155,12,120);
    setGroupOf3(2, 3, 155,12,120);
    setGroupOf3(3, 6, 155,12,120);  
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
 
  // Start Midi
  Serial.begin(31250);
  
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
  
  // set channels / patches
  for (int ch=0; ch<melody_count; ch++){
    
    byte the_chan =  pgm_read_byte(&(chan_voices[ch][chann]));
    byte the_voice = pgm_read_byte(&(chan_voices[ch][voice]));
    program_change(the_chan, the_voice);
    
    // All notes and sound off for this channel (to stop stuck notes if reset)
    midimsg( 0xB0 | the_chan, 0x78, 0);
    midimsg( 0xB0 | the_chan, 0x7B, 0);
  }

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
  
  // First, deal with any Note Offs which need to be sent.
  check_noteoffs(tick_counter);
 
   // ShiftMatrixPWM
    for(int row=0;row<numRows;row++){
    for(int col=0;col<numColumns-1;col++){
      for(int brightness=0;brightness<maxBrightness;brightness++){
        ShiftMatrixPWM.SetOne(row, col+1,brightness);
        ShiftMatrixPWM.SetOne(row,col,maxBrightness-brightness);
        delay(3);
        active();
      }
    }
  }
  
  // Update current position in the bar, 0 -> 31
   if( runMode == 1){position = tick_counter % tpb;}
 
 
   // check the keyboard 
  localKey = keypad.getKey();
  if (localKey != SAMPLE_WAIT){
      
     if(runMode == 0 && editMode == 0){ 
       switch (localKey) {
  	case 3: menu.moveUp(); break;
  	case 4: menu.moveDown(); break;
  	case 5: menu.moveRight(); break;
  	case 2: menu.moveLeft(); break;
  	case 1: menu.use(); break;
       }
       
     } else if (runMode == 1 && editMode == 0){

       switch (localKey) {
  	case 3: previousChannel(); break;                // up = prev channel
  	case 4: nextChannel(); break;              // down = next channel
        // these will be to move to the next bar
  	//case 5: menu.moveRight(); break;
  	//case 2: menu.moveLeft(); break;
  	case 1: stopRun();  break;  // Select button = STOP
       }   
     } else if ( runMode == 0 && editMode == 1){ 
       switch (localKey) {
  	case 3: previousChannel(); break;                // up = prev channel
  	case 4: nextChannel(); break;              // down = next channel
        case 5: editNextStep(); break;	
        case 2: editPreviousStep(); break;
  	case 1: stopEdit();  break;  // Select button = STOP       
       } 
     }
   
  }

  if (runMode == 0 && editMode == 1){ 
    lcd.print("!"); 
    printCurrentPosition(position);
     printCurrentChannel();
     
      for (int current_mel=first_mel; current_mel<melody_count+1; current_mel++){
    
        // Pointer to current melody
        prog_uchar * current_mel_p = (prog_uchar*) pgm_read_word(&song[bar_counter][current_mel]);
        

        byte this_beat = pgm_read_byte(&(current_mel_p[currentChannel+mel_beat]));
       
      
          if (this_beat == position) // Note needs to be played here!
          {
           byte this_note =     pgm_read_byte(&(current_mel_p[currentChannel+mel_note]));
           byte this_duration = pgm_read_byte(&(current_mel_p[currentChannel+mel_duration]));
           byte this_velocity = pgm_read_byte(&(current_mel_p[currentChannel+mel_velocity]));
     
           lcd.print(" N");
           lcd.print(this_note);
           lcd.print(" D");
           lcd.print(this_duration);
           lcd.print(" V");
           lcd.print(this_velocity);
           lcd.print(" ");
          }
     
      }
      
  } else if (runMode == 1 && editMode == 0){  
      lcd.clear();
     printCurrentPosition(position);
     printCurrentChannel();
     
  //Process MELODIES
  // Loop through each melody
  for (int current_mel=first_mel; current_mel<melody_count+1; current_mel++){
    
    // Pointer to current melody
    prog_uchar * current_mel_p = (prog_uchar*) pgm_read_word(&song[bar_counter][current_mel]);
    
    // Channel number for current melody
    byte current_chan = pgm_read_byte(&(chan_voices[current_mel-first_mel][chann]));

    int index = 0;
    byte this_beat = pgm_read_byte(&(current_mel_p[4*index+mel_beat]));
   

      if (this_beat == position) // Note needs to be played here!
      {
        byte this_note =     pgm_read_byte(&(current_mel_p[4*index+mel_note]));
        byte this_duration = pgm_read_byte(&(current_mel_p[4*index+mel_duration]));
        byte this_velocity = pgm_read_byte(&(current_mel_p[4*index+mel_velocity]));
        
        noteon(current_chan, this_note, this_velocity);
        add_noteoff(this_note, current_chan, tick_counter+this_duration);
        if (current_chan == currentChannel){
          lcd.setCursor(4,1);
          lcd.print(this_note);
          lcd.print("-");
          lcd.print(this_velocity);
        }

    
      index++;
      this_beat = pgm_read_byte(&(current_mel_p[4*index+mel_beat]));
    }

  }
  
 
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
    while (millis() < start_time + pause) {} 
  }
  
   
}

//-----------------------------------------------------------
// Functions to handle the Note Off arrays.
//-----------------------------------------------------------

// Add a note to the noteoff arrays
void add_noteoff(byte note, byte chan, unsigned int time){
  int i = 0;
  while(noteoff_times[i]) i++; // find an empty slot
  
  if (i == noteoff_size) // array is full!
  {
    //digitalWrite(led_pin,HIGH);
  }
  else
  {
    noteoff_times[i] = time;
    noteoff_notes[i] = note;
    noteoff_chans[i] = chan;
  //  digitalWrite(led_pin, LOW);
  }
}

// Checks the noteoff arrays and sends appropriate noteoffs
void check_noteoffs(unsigned int couter)
{
  for (int i = 0; i<noteoff_size; i++)
  {
    if (noteoff_times[i] == tick_counter)
    {
      noteoff(noteoff_chans[i], noteoff_notes[i]);
      noteoff_times[i] = 0; // slot is now empty
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
  }
}

void nextChannel(){ 
  if (currentChannel <= melody_count){
    currentChannel++;  
    printCurrentChannel();
  }
}

void editPreviousStep(){
  if (position > 0){ 
     if ( position != 0 && bar_counter !=0){
       position--;
     } else { 
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
