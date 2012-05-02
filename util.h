/*
    Cahors, by Jamie Matthews.
    A MIDI sequencer for the Arduino physical computing platform

    This file is part of Cahors.
    
    Cahors is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cahors is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cahors.  If not, see <http://www.gnu.org/licenses/>.
*/
#define  END          251
 // invalid MIDI note number, used to mark end of pattern
prog_uchar SONGEND PROGMEM = END; // used to mark end of song


// Channel array offsets
#define  chann         0
#define  voice         1

// drum pattern offsets
#define  pat_note      0 // MIDI note number
#define  pat_velocity  1 // MIDI velocity (volume)
#define  pat_duration  2 // duration of each note
#define  pat_b1        3 // The four data bytes..
#define  pat_b2        4 // ..
#define  pat_b3        5 // ..
#define  pat_b4        6 // ..

// melody pattern offsets
#define  mel_beat      0
#define  mel_note      1
#define  mel_velocity  2
#define  mel_duration  3

// Song array offsets
#define  drums          0
#define  first_mel      1

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




