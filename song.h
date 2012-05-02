
/*
    cerealdata, a composition for Cahors by Jamie Matthews.
    
    This composition is licensed under a Creative Commons 
    Attribution-Noncommercial-Share Alike 3.0 License
    
    Please see http://creativecommons.org/licenses/by-nc-sa/3.0/
*/

#define bpm 128
#define melody_count  4

#define  mel1chan     0
#define  mel2chan     1
#define  mel3chan     2
#define  mel4chan     3

// Voices used
#define  sawlead      81
#define  synbass      33
#define  bowed_pad    92
#define  ovrdrv       80
// Notes used
#define  note_kick    35
#define  note_crash2  57
#define  note_hihat   44
#define  note_snare   38


// Channels for each part.
#define  drumchan     9 // should normally be 9
// Channel -> Voice mappings
prog_uchar chan_voices[melody_count][2] PROGMEM =
{
  {  mel1chan,  bowed_pad  },
  {  mel2chan,   synbass   },
  {  mel3chan,   sawlead   },
  {  mel4chan,   ovrdrv    }
};

//---------------------------------------------------
///////////////// EMPTY PATTERNS ////////////////////
//---------------------------------------------------
prog_uchar empty_drums[4][7] PROGMEM =
  { 
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  } 
  };
  
prog_uchar empty_mel[][4] PROGMEM = 
  {
    {  END,END,    END,  END}
  };

//---------------------------------------------------
///////////////// PATTERN ARRAYS ////////////////////
//---------------------------------------------------

// Drums

prog_uchar drums_a[4][7] PROGMEM =
  { 
    {  note_kick, 120,  1,  0x80,  0x00,  0x0f,  0xc0  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  }
  };
  
prog_uchar drums_b[4][7] PROGMEM =
  { 
    {  note_kick, 120,  1,  0x80,  0x82,  0xfc,  0x80  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  }
  };
  
prog_uchar drums_c[4][7] PROGMEM =
  { 
    {  note_kick, 120,  1,  0x8a,  0xf4,  0x2c,  0x0e  },
    {  note_hihat,120,  1,  0x88,  0x80,  0xa8,  0x80  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  }
  };
  
prog_uchar drums_d[4][7] PROGMEM =
  { 
    {  note_kick, 120,  1,  0x80,  0x80,  0x80,  0x80  },
    {  note_snare,120,  1,  0x08,  0x08,  0x08,  0x08  },
    {  note_hihat,120,  1,  0x88,  0x80,  0xa8,  0x80  },
    {  0, 0, 1,  0x00,  0x00,  0x00,  0x00  }
  };

   
// Melodies
prog_uchar mel_a[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar mel_b[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  0,  Cs+oct3,100,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar mel_c[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  0,  Cs+oct3,100,  64 },
    {  0,  A+oct3, 100,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar mel_d[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  0,  Cs+oct3,100,  64 },
    {  0,  A+oct3, 100,  64 },
    {  0,  Gs+oct3,100,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar mel_e[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  64 },
    {  0,  Cs+oct3,100,  64 },
    {  0,  A+oct3, 100,  64 },
    {  0,  Gs+oct3,100,  64 },
    {  0,  F+oct3, 100,  64 },
    {  END,END,    END,  END}
  };

prog_uchar mel_f[][4] PROGMEM = 
  {
    {  0,  C+oct3, 100,  128},
    {  0,  Cs+oct3,100,  128},
    {  0,  A+oct3, 100,  128},
    {  0,  Gs+oct3,100,  128},
    {  0,  F+oct3, 100,  128},
    {  0,  Fs+oct3,100,  128},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_g[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4},
    {  4,  E+oct4, 120,  4},
    {  8,  Fs+oct4,120,  4},
    {  12, A+oct4, 120,  4},
    {  16, C+oct5, 120,  16},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_h[][4] PROGMEM = 
  {
    {  0,  G+oct4, 120,  4},
    {  4,  A+oct4, 120,  4},
    {  8,  E+oct4, 120,  4},
    {  12, F+oct4, 120,  4},
    {  16, C+oct4, 120,  16},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_i[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4},
    {  4,  E+oct4, 120,  4},
    {  8,  Fs+oct4,120,  4},
    {  12, A+oct4, 120,  4},
    {  16, E+oct5, 120,  16},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_j[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4},
    {  4,  E+oct4, 120,  4},
    {  8,  Fs+oct4,120,  4},
    {  12, A+oct4, 120,  4},
    {  16, G+oct5, 120,  16},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_k[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4},
    {  4,  E+oct4, 120,  4},
    {  8,  Fs+oct4,120,  4},
    {  12, A+oct4, 120,  4},
    {  16, B+oct5, 120,  16},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_l[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4},
    {  4,  E+oct4, 120,  4},
    {  8,  Fs+oct4,120,  4},
    {  12, A+oct4, 120,  4},
    {  16, C+oct6, 120,  4},
    {  20, C+oct6, 120,  4},
    {  24, C+oct6, 120,  4},
    {  28, C+oct6, 120,  4},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_m[][4] PROGMEM = 
  {
    {  0, C+oct6, 120,  4},
    {  0, B+oct5, 120,  4},
    {  4, C+oct6, 120,  4},
    {  4, B+oct5, 120,  4},
    {  8, C+oct6, 120,  4},
    {  8, B+oct5, 120,  4},
    {  12,C+oct6, 120,  4},
    {  12,B+oct5, 120,  4},
    {  16,C+oct6, 120,  4},
    {  16,B+oct5, 120,  4},
    {  16,As+oct5,120,  4},
    {  20,C+oct6, 120,  4},
    {  20,B+oct5, 120,  4},
    {  20,As+oct5,120,  4},
    {  24,C+oct6, 120,  4},
    {  24,B+oct5, 120,  4},
    {  24,As+oct5,120,  4},
    {  28,C+oct6, 120,  4},
    {  28,B+oct5, 120,  4},
    {  28,As+oct5,120,  4},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_n[][4] PROGMEM = 
  {
    {  0,  G+oct4, 120,  4},
    {  8,  A+oct4, 120,  4},
    {  16,  E+oct4, 120, 4},
    {  24, F+oct4, 120,  4},
    {  END,END,    END,  END}
  };
  
prog_uchar mel_o[][4] PROGMEM = 
  {
    {  0,  C+oct2, 100,  255},
    {  0,  C+oct3, 60,   128},
    {  0,  E+oct3, 60,   128},
    {  0,  G+oct3, 60,   128},
    {  0,  C+oct4, 100,  96},
    {  0,  E+oct4, 100,  96},
    {  0,  G+oct4, 100,  96},
    {  0,  C+oct5, 120,  64},
    {  0,  E+oct5, 120,  64},
    {  0,  G+oct5, 120,  64},
    {  END,END,    END,  END}
  };
  
  
// bass
prog_uchar bass_a[][4] PROGMEM = 
  {
    {  0,  C+oct6, 120,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar bass_b[][4] PROGMEM = 
  {
    {  0,  Cs+oct5,120,  64 },
    {  END,END,    END,  END}
  };
  
prog_uchar bass_c[][4] PROGMEM = 
  {
    {  0,  C+oct4, 120,  4  },
    {  4,  C+oct3, 120,  4  },
    {  8,  C+oct4, 120,  4  },
    {  12, C+oct3, 120,  4  },
    {  16, C+oct5, 120,  4  },
    {  20, C+oct5, 120,  4  },
    {  24, C+oct5, 120,  4  },
    {  28, C+oct5, 120,  4  },
    {  END,END,    END,  END}
  };
  
prog_uchar bass_d[][4] PROGMEM = 
  {
    {  0,  C+oct3, 120,  4  },
    {  4,  C+oct2, 120,  4  },
    {  8,  C+oct3, 120,  4  },
    {  12, C+oct2, 120,  4  },
    {  16, C+oct4, 120,  4  },
    {  20, C+oct4, 120,  4  },
    {  24, C+oct4, 120,  4  },
    {  28, C+oct4, 120,  4  },
    {  END,END,    END,  END}
  };
  
    
//---------------------------------------------------
//////// THE SONG ARRANGMENT ARRAY //////////////////
//---------------------------------------------------
prog_uchar *song[][melody_count+1] PROGMEM = 
  { //     DRUMS               | MEL1                 | MEL2               | MEL3               | MEL4
  
    // Chords intro
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_a[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_b[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_c[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_d[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_e[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_f[0][0],        &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    
    // random-ish weird bit
    {  &drums_a[0][0],         &bass_a[0][0],         &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_a[0][0],         &bass_b[0][0],         &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_b[0][0],         &bass_a[0][0],         &bass_a[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_a[0][0],         &bass_b[0][0],         &bass_b[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_b[0][0],         &bass_a[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_b[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_b[0][0],         &bass_a[0][0],         &bass_a[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_a[0][0],         &bass_b[0][0],         &bass_b[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_c[0][0],         &bass_a[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_a[0][0],         &bass_b[0][0],         &bass_b[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &bass_a[0][0],         &bass_c[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    // back to the nasty chords, with drums
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_i[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_j[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_k[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_l[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_l[0][0],        &bass_d[0][0],       &mel_l[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_m[0][0],        &bass_d[0][0],       &mel_m[0][0],     },
    
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_l[0][0],        &bass_d[0][0],       &mel_l[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_m[0][0],        &bass_c[0][0],       &mel_m[0][0],     },
    
    // nasty chords, no drums
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_a[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_b[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_c[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_d[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_g[0][0],        &bass_d[0][0],       &mel_g[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_h[0][0],        &bass_d[0][0],       &mel_h[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_l[0][0],        &bass_d[0][0],       &mel_l[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &mel_m[0][0],        &bass_c[0][0],       &mel_m[0][0],     },
    
    // back to the nasty chords, with drums
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &mel_b[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    // nasty chords, drums, lead
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &mel_b[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_g[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_i[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_b[0][0],        &mel_j[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &mel_k[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &mel_l[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_a[0][0],        &mel_i[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &mel_b[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_c[0][0],        &mel_j[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_d[0][0],        &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &mel_k[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_h[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_l[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &mel_f[0][0],        &mel_l[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_c[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_l[0][0],     },
    {  &drums_d[0][0],         &empty_mel[0][0],      &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &drums_d[0][0],         &bass_a[0][0],         &bass_d[0][0],       &mel_f[0][0],        &mel_l[0][0],     },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    {  &drums_d[0][0],         &bass_a[0][0],         &bass_d[0][0],       &empty_mel[0][0],    &mel_l[0][0],     },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &drums_d[0][0],         &bass_a[0][0],         &bass_d[0][0],       &mel_f[0][0],        &mel_l[0][0],     },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    {  &drums_c[0][0],         &bass_b[0][0],         &mel_l[0][0],        &empty_mel[0][0],    &mel_l[0][0],     },
    {  &drums_d[0][0],         &bass_b[0][0],         &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &drums_c[0][0],         &bass_a[0][0],         &bass_d[0][0],       &mel_f[0][0],        &mel_l[0][0],     },
    {  &drums_a[0][0],         &bass_b[0][0],         &bass_c[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    {  &drums_c[0][0],         &bass_b[0][0],         &mel_l[0][0],        &mel_m[0][0],        &mel_l[0][0],     },
    {  &drums_b[0][0],         &bass_b[0][0],         &bass_d[0][0],       &empty_mel[0][0],    &mel_m[0][0],     },
    
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_l[0][0],        &mel_l[0][0],        &mel_l[0][0],     },
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_m[0][0],        &mel_m[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_l[0][0],        &mel_l[0][0],        &mel_l[0][0],     },
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_m[0][0],        &mel_m[0][0],        &mel_m[0][0],     },
    
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_l[0][0],        &mel_l[0][0],        &mel_l[0][0],     },
    {  &empty_drums[0][0],     &bass_a[0][0],         &mel_m[0][0],        &mel_m[0][0],        &mel_m[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_g[0][0],        &mel_g[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_n[0][0],        &mel_n[0][0],     },
    
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &mel_o[0][0],        &mel_o[0][0],     },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &empty_drums[0][0],     &empty_mel[0][0],      &empty_mel[0][0],    &empty_mel[0][0],    &empty_mel[0][0]  },
    {  &SONGEND,               &SONGEND,              &SONGEND,            &SONGEND,            &SONGEND }
  };

