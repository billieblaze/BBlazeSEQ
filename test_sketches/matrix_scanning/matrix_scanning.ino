// lunttaa http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1202874493

// ard   595
// d0    midi out
// d1    midi in
// d2    sync in
// d3    btn595 shcp/11
// d4    btn595 stcp/12
// d5    btn595 ds/14
// d6    btn165 ce/15
// d7    btn165 cp/2
// d8    btn165 pl/1
// d9    btn165 q7/9
// d10   led595 ds/14
// d11   led595 shcp/11
// d12   led595 stcp/12
// d13   (sync out?)
// 14/a0 notedec
// 15/a1 noteinc
// 16/a2 notepus
// 17/a3 lcd595 14/ds
// 18/a4 lcd595 11/shcp
// 19/a5 lcd595 12/stcp
#define MIDI
#define SHIFTOUT
/*#define EXT_SYNC*/

#define INT_MS 1

// 595 for leds
#define led595dat_pin 11
#define led595clk_pin 13
#define led595str_pin 9


uint8_t led595data[] = {255, 0};

// push byte to shift register and on to LEDs
void
pushOut(uint8_t *value, int count) {
  digitalWrite(led595str_pin, LOW);
  for(int i=0;i<count;i++) {
      shiftOut(led595dat_pin, led595clk_pin, MSBFIRST, value[i]);
  }

  digitalWrite(led595str_pin, HIGH);
}


/*extern "C" void __cxa_pure_virtual() {}*/

const byte COLS = 4;
const byte ROWS = 4;
/*const byte PADROWS = 8;*/

volatile byte column = 0;
volatile bool evenRow = 1;

bool leds[ROWS][COLS] = {0};

  
  

void
setup()
{


    // 595
    pinMode(led595dat_pin,OUTPUT);
    pinMode(led595clk_pin,OUTPUT);
    pinMode(led595str_pin,OUTPUT);


}

int ledStatus = 0;

void
loop()
{
 shiftOut(led595dat_pin, led595clk_pin, MSBFIRST, 44);
 shiftOut(led595dat_pin, led595clk_pin, MSBFIRST, 100);
 shiftOut(led595dat_pin, led595clk_pin, MSBFIRST, 244);
}

// vim: ft=arduino

