/*
  7-Segment LED counter, multiplexing using 74HC595 8-bit shift
  register.

  Displays a digit by illuminating each individual segment that makes
  it up on the display sequentially, rather than illuminating each
  segment at once. This is done very quickly (multiplexing) to give
  the illusion that all the nesersary segments are illuminated at
  once. This is because the 74HC595 can’t source or sink that much
  current, and lighting all segments for the digit ‘8′ at once for
  example could require too much current and damage the chip.

  Connections

    Vcc = 3.3v on Arduino

    Arduino pin 5 => 74HC595 pin 12
    Arduino pin 6 => 74HC595 pin 14
    Arduino pin 7 => 74HC595 pin 11

    74HC595 pin 1  (Q1)   => LED Pin 6  (B)
    74HC595 pin 2  (Q2)   => LED Pin 4  (C)
    74HC595 pin 3  (Q3)   => LED Pin 2  (D)
    74HC595 pin 4  (Q4)   => LED Pin 1  (E)
    74HC595 pin 5  (Q5)   => LED Pin 9  (F)
    74HC595 pin 6  (Q6)   => LED Pin 10 (G)
    74HC595 pin 7  (Q7)   => LED Pin 5  (DP)
    74HC595 pin 8  (GND)  => Ground
    74HC595 pin 9  (Q7S)  => Not connected
    74HC595 pin 10 (MR)   => Vcc (High)
    74HC595 pin 11 (SHCP) => Arduino pin 7
    74HC595 pin 12 (STCP) => Arduino pin 5
    74HC595 pin 13 (OE)   => Ground (Low)
    74HC595 pin 14 (DS)   => Arduino pin 6
    74HC595 pin 15 (Q0)   => LED Pin 7  (A)
    74HC595 pin 16 (Vcc)  => Vcc

    LED pin 3 or 8 => 220 Ohm resistor => Vcc

    Created 6 Nov 2011
    by Mark Sweeting - www.sweeting.org/mark
 */

const int latchPin = 5;  // Pin connected to Pin 12 of 74HC595 (Latch)
const int dataPin  = 6;  // Pin connected to Pin 14 of 74HC595 (Data)
const int clockPin = 7;  // Pin connected to Pin 11 of 74HC595 (Clock)

unsigned long t1;
unsigned long t2;
int i = 0;

// Describe each digit in terms of display segments
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
const byte numbers[16] = {
                    0b11111100,
                    0b01100000,
                    0b11011010,
                    0b11110010,
                    0b01100110,
                    0b10110110,
                    0b10111110,
                    0b11100000,
                    0b11111110,
                    0b11100110,
                    0b11101110,
                    0b00111110,
                    0b10011100,
                    0b01111010,
                    0b10011110,
                    0b10001110
};

void setup()
{

  // initialisation time
  t1 = millis();

  //set pins to output 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop()
{
    // update digit every two seconds
    t2 = millis();
    if(t2 - t1 > 2000)
    {
      i++;
      t1 = t2;
      if(i > 15) { i = 0; }
    }
    // display the current digit
    show(numbers[i]);
}

void show( byte number)
{
  // Use a loop and a bitwise AND to move over each bit that makes up
  // the seven segment display (from left to right, A => G), and check
  // to see if it should be on or not
  for(int j = 0; j <= 7; j++)
  {
    byte toWrite = number & (0b10000000 >> j); 

    // If all bits are 0 then no point writing it to the shift register,
    // so break out and move on to next segment.
    if(!toWrite) { continue; }

    // Otherwise shift it into the register
    shiftIt(toWrite);
  }
}

void shiftIt (byte data)
{
    // Set latchPin LOW while clocking these 8 bits in to the register
    digitalWrite(latchPin, LOW);

    for (int k=0; k <= 7; k++)
    {
      // clockPin LOW prior to sending a bit
      digitalWrite(clockPin, LOW); 

      // Note that in our case, we need to set pinState to 0 (LOW) for
      // “On” as the 74HC595 is sinking current when using a common
      // anode display. If you want to use a common cathode display then
      // switch this around.
      if ( data & (1 << k) )
      {
        digitalWrite(dataPin, LOW); // turn “On”
      }
      else
      {
        digitalWrite(dataPin, HIGH); // turn “Off”
      }

      // and clock the bit in
      digitalWrite(clockPin, HIGH);
    }

    //stop shifting out data
    digitalWrite(clockPin, LOW); 

    //set latchPin to high to lock and send data
    digitalWrite(latchPin, HIGH);

    // put delay here if you want to see the multiplexing in action!
    //delay(100);
}
