/*
LED Matrix Tester
*/

#include "SPI.h"
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <DFR_Key.h>

  
#define  noteoff_size 32   // size of the noteoff arrays in memory
#define  mask         0x80 // used for drum patterns. b10000000

String version = "0.3";

// Init shiftMatrixPWM LED Matrix

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
int numColumnRegisters =4;
int numRows=4;

int numColumns = numColumnRegisters*4;
int numOutputs = numColumns*numRows;

int j = 0;
int row = 0;
int col = -1;
int r = 150;
int g = 150;
int b = 150;



int localKey = 0; 
String keyString = "";


//shiftPWM
void setGroupOf3(int row, int start, int r, int g, int b){
     ShiftMatrixPWM.SetOne(row, start, r);
     ShiftMatrixPWM.SetOne(row, start+1, g);
     ShiftMatrixPWM.SetOne(row, start+2, b);
}


// SETUP                 
void setup() { 



  // setup keys

   
  
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


}
void  nextNode(){
       setGroupOf3(row,col,0,0,0);
  if (col < 3){ 
    col++;
  } else { 
    if(row <4 ) { 
    row++;
    } else { 
      row=0;
    };
    col = 0;
  }
   
         setGroupOf3(row,col*3, 255,0,0);
    delay(1500);
             setGroupOf3(row,col, 0,255,0);
    delay(1500);
              setGroupOf3(row,col, 0,0,255);
    delay(1500);
         setGroupOf3(row,col*3, 255,255,0);
    delay(1500);
    
         setGroupOf3(row,col*3, 0,255,255);
    delay(1500);
    
         setGroupOf3(row,col*3, 255,0,255);
    delay(1500);
         setGroupOf3(row,col*3, 255,255,255);
    delay(1500);

}


void loop() { 
  
   nextNode();  
}
