#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

#include "SPI.h"
#include <MIDI.h>
#include <digitalWriteFast.h>
boolean Started = false;
boolean badClock = false;
unsigned long startTime;
unsigned long currentTime;
unsigned long processedtime;
float BPM = 94.0f;
float PPQ = 36.0f;
float ppqtime;
float currentmicroTime;
float ppqmicroTime;
int beats = 0;
int ppqcounter = 0;
 int gate[] = { 45,43,41, 39 }; 
 
void setup()
{
  Serial.begin(115200);
  MIDI.begin();
  MIDI.turnThruOn();
}

void loop()
{
  if (!Started)
  {
	Started = true;
	ppqtime = 60000000.0f / (BPM*PPQ);  // (60 x 1,000,000) / (BPM*PPQ)
	Serial.print("BPM: ");
	Serial.println(BPM);
	Serial.print("PPQ: ");
	Serial.println(PPQ);
	Serial.print("Micros PPQ: ");
	Serial.println(ppqtime);
	beats = 0;
	ppqcounter = 0;
	startTime = millis();
	ppqmicroTime = (float)micros() + ppqtime;
      MIDI.sendRealTime(Start);
       MIDI.sendNoteOn(50,100,1);
  }

  currentmicroTime = (float)micros();
  if (currentmicroTime >= ppqmicroTime)
  {
    currentTime = millis();
    ppqmicroTime += ppqtime;
    ppqcounter++;
    MIDI.sendRealTime(Clock);

    if (ppqcounter == PPQ)
    {
      
  digitalWriteFast(gate[1], HIGH);
    digitalWriteFast(gate[1], LOW);
   MIDI.sendNoteOn(50,100,1);
  
     
     
        Serial.println("BEAT");
        Serial.println(beats);
     //   delay(20);
     
   
        
      	ppqcounter = 0;
	beats++;
	/*if (beats == BPM)
	{
	  processedtime = currentTime - startTime;
	  Serial.println("All BPM Done!");
	  Serial.print("Processed Time MS: ");
	  Serial.println(processedtime);
	  BPM += 25;
	  Started = false;
	}*/
    }  
  
  }
} 

