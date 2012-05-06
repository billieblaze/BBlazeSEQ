
/*
    cerealdata, a composition for Cahors by Jamie Matthews.
    
    This composition is licensed under a Creative Commons 
    Attribution-Noncommercial-Share Alike 3.0 License
    
    Please see http://creativecommons.org/licenses/by-nc-sa/3.0/
*/

#define bpm 128
#define melody_count  1

#define  mel1chan     0

// Voices used
#define  sawlead      81
#define  synbass      33
#define  bowed_pad    92
#define  ovrdrv       80

// Channel -> Voice mappings
prog_uchar chan_voices[melody_count][2] PROGMEM =
{
  {  mel1chan,  bowed_pad  }
};



//---------------------------------------------------
///////////////// PATTERN ARRAYS ////////////////////
//---------------------------------------------------

   
// Melodies
prog_uchar mel_a[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  END,END,    END,  END}
  };

  
//---------------------------------------------------
//////// THE SONG ARRANGMENT ARRAY //////////////////
//---------------------------------------------------
prog_uchar *song[][melody_count] PROGMEM = 
  {  
    // Chords intro
    {       &mel_a[0][0] }
  };
