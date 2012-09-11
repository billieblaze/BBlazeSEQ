 int MIDIClockCounter = 0;
    int tickCounter = 0;
    int runSequencer=0;
    int editMode=0;
    int editParam=0;


  int currentChannel = 0;  // which channel are we viewing?
    int currentStep = 0;


    int lastNote[4] = {0,0,0,0};
    int recordLastNote = 0;
    int recordLastPosition = 0;
    int curPosition = 0;
    int keyOctave = 3;
    
void playNotes(){
  for(int channel=0; channel <= channels; channel++){
    int noteOn = patternData[channel][0][tickCounter];
    int note = patternData[channel][1][tickCounter];
    int velocity = patternData[channel][2][tickCounter];

    if (noteOn == 0 ){ 
      MIDI.sendNoteOff( lastNote[channel],0,channel+1);
      digitalWriteFast(gate[channel ], LOW );
    }

    if ( noteOn == 1 ) { 
      MIDI.sendNoteOn( note, velocity,channel+1);
       digitalWriteFast(gate[channel], HIGH);
      lastNote[channel] = note;
    }
    
    //  if (noteOn == 2) hold the note plz   
   
  } 
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
  updateMatrix=1;
}

void HandleStop(){  
  runSequencer = 0; 
  for ( int i = 0; i < channels; i++){
  MIDI.sendNoteOff(lastNote[i],0, i+1);
  digitalWriteFast(gate[i], LOW);
  }
}

void HandleClock(){

  if(  runSequencer == 1){

    // play notes every time the clock divider rolls over
    if (MIDIClockCounter == 0){
       playNotes();
      updateMatrix=1;
      tickCounter++;
    } 

    MIDIClockCounter++;

    // bar complete - start over  
    if(tickCounter == 32){ 
      tickCounter=0;
    } 

    // reset the click divider
    if (MIDIClockCounter == 6){ 
      MIDIClockCounter=0; 
    } 
  }

}


// Setup Midi events
void setupMIDI(){
  MIDI.begin();
  MIDI.setHandleStart(HandleStart);
  MIDI.setHandleClock(HandleClock);
  MIDI.setHandleStop(HandleStop);
  MIDI.setHandleContinue(HandleStart);
 // MIDI.turnThruOff();  
 
 for ( int i = 0; i < channels; i++){ 
    pinMode(gate[i], OUTPUT); 
 }
}

