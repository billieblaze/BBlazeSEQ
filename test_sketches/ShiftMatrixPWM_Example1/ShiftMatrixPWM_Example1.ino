/******************************************************************************
 * //Comments yet to to be updated 
 * 
 * (c) Elco Jacobs, Sept 2011.
 * 
 *****************************************************************************/
//#include <Servo.h>
#include <SPI.h>
#include "hsv2rgb.h"


//Data pin is MOSI (atmega168/328: pin 11. Mega: 51) 
//Clock pin is SCK (atmega168/328: pin 13. Mega: 52)
const int ShiftMatrixPWM_columnLatchPin=49;
const int ShiftMatrixPWM_rowDataPin=A15;
const int ShiftMatrixPWM_rowClockPin=A14;
const int ShiftMatrixPWM_rowLatchPin=A13;

const bool ShiftMatrixPWM_invertColumnOutputs = 0; // if invertColumnOutputs is 1, outputs will be active low. Usefull for common anode RGB led's.

const bool ShiftMatrixPWM_invertRowOutputs = 1; // if invertOutputs is 1, outputs will be active low. Used for PNP transistors.

#include <ShiftMatrixPWM.h>   // include ShiftMatrixPWM.h after setting the pins!


unsigned char maxBrightness = 63;
unsigned char pwmFrequency = 60;
int numColumnRegisters = 3;
int numRows=4;

int numColumns = numColumnRegisters*8;
int numOutputs = numColumns*numRows;


void setup()   {                
  pinMode(ShiftMatrixPWM_columnLatchPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowDataPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowClockPin, OUTPUT); 
  pinMode(ShiftMatrixPWM_rowLatchPin, OUTPUT); 
 
 
  
  SPI.setBitOrder(LSBFIRST);
  // SPI_CLOCK_DIV2 is only a tiny bit faster in sending out the last byte. 
  // SPI transfer and calculations overlap for the other bytes.
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 

  Serial.begin(9600);
Serial.print("START");

  ShiftMatrixPWM.SetMatrixSize(numRows, numColumnRegisters);
  ShiftMatrixPWM.Start(pwmFrequency,maxBrightness);  
}



void loop()
{    
  // Print information about the interrupt frequency, duration and load on your program
  ShiftMatrixPWM.SetAll(0);
  ShiftMatrixPWM.PrintInterruptLoad();

  // Fade in and fade out all outputs one by one fast. Usefull for testing your circuit
//ShiftMatrixPWM.OneByOneFast();  
  
  // Fade in all outputs
 /* for(int j=0;j<maxBrightness;j++){
    ShiftMatrixPWM.SetAll(j);  
    delay(2000);
  }
  // Fade out all outputs
  for(int j=maxBrightness;j>=0;j--){
    ShiftMatrixPWM.SetAll(j);  
    delay(2000);
  }

*/
  // Fade in and out  outputs one at a time
  for(int row=0;row<4;row++){
    for(int col=0;col<8;col++){
      ShiftMatrixPWM.SetAll(0);
      
   
        setGroupOf3(row,col*3,255,255,255);
        delay(100);
        
      
    }
  }

  //  A moving rainbow for RGB leds:
  //rgbLedRainbow(numOutputs/3, 5, 3, maxBrightness, numColumns/3); // Fast, over all LED's, 1 column wide rainbow
  rgbLedRainbow(numOutputs/3, 10, 3, maxBrightness, numOutputs); //slower, over all LED's, as wide as the whole matrix

  // Fade in and fade out all outputs slowly. Usefull for testing your circuit
  ShiftMatrixPWM.OneByOneSlow();  

}
  void setGroupOf3(int row, int start, int r, int g, int b){
    ShiftMatrixPWM.SetOne(row, start, r);
    ShiftMatrixPWM.SetOne(row, start+1, g);
    ShiftMatrixPWM.SetOne(row, start+2, b);
  }
void rgbLedRainbow(int numRGBLeds, int delayVal, int numCycles, int maxBrightness, int rainbowWidth){
  // Displays a rainbow spread over all LED's, which shifts in hue.
  int hue, sat, val; 
  unsigned char red, green, blue;

  ShiftMatrixPWM.SetAll(0);
  for(int cycle=0;cycle<numCycles;cycle++){ // shift the raibom numCycles times
    for(int colorshift=0;colorshift<360;colorshift++){ // Shift over full color range (like the hue slider in photoshop)
      for(int led=0;led<numRGBLeds;led++){ // loop over all LED's
        hue = ((led)*360/(rainbowWidth-1)+colorshift)%360; // Set hue from 0 to 360 from first to last led and shift the hue
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        int group = led%(numColumns/3);
        int row = (led - group)/(numColumns/3);
        setGroupOf3(row, group*3, red, green, blue); // write rgb values
      }
      delay(delayVal);
    } 
  }  
}


