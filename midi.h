

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
}

