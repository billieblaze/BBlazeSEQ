/*		LCDi2cR Library for the LCD display from robot-electronics.co.uk
 
		Version 0.1		4/2/2009
 
		Arduino Library maintained by Dale Wentz  dale@wentztech.com
 
		For the latest version see
 
*/



#include "LCDi2cR.h"

#include <Wire.h>

#include <inttypes.h>

#include "Arduino.h"  //all things wiring / arduino

#define _LCDEXPANDED				// If defined turn on advanced functions

// Global Vars 

uint8_t g_num_lines = 4;
uint8_t g_num_col = 20;
int g_i2caddress = 0x63;
uint8_t g_display = 0;
int g_cmdDelay = 0;
int g_charDelay = 0;



// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	LCDi2c Class
// []
// []	num_lines = 1-4
// []   num_col = 1-80
// []   i2c_address = 7 bit address of the device
// []   display = Not used at this time
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


LCDi2cR::LCDi2cR(uint8_t num_lines,uint8_t num_col,uint8_t i2c_address,uint8_t display){
	
	g_num_lines = num_lines;
	g_num_col = num_col;
	g_i2caddress = i2c_address;
	g_display = display;
	
	//if (g_num_lines < 1 || g_num_lines > 4)
		//g_num_lines = 2;
	
	//if (g_num_col < 1 || g_num_col > 40)
		//g_num_col = 16;
	
}





// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	initiatize lcd after a short pause
// []
// []	Init the i2c buss
// []   Put the display in some kind of known mode
// []   Put the cursor at 0,0
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::init () {
	
	Wire.begin();
	
	on();
	clear();
	blink_off();
	cursor_off(); 
	home();
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Over ride the default delays used to send commands to the display
// []
// []	The default values are set by the library
// []   this allows the programer to take into account code delays
// []   and speed things up.
// []   
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::setDelay (int cmdDelay,int charDelay) {
	
	g_cmdDelay = cmdDelay;
	g_charDelay = charDelay;
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []   Send a command to the display. 
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]



void LCDi2cR::command(uint8_t value) {
	
	Wire.beginTransmission(g_i2caddress);
	Wire.send(0x00);
	Wire.send(value);
	Wire.endTransmission();
	//delay(g_charDelay);
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []   Send a command to the display. 
// []
// []	This is also used by the print, and println
// []	
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::write(uint8_t value) {
	
	// This display maps custom characters from 128-135 Dec, most displays map them to 0-7 so lets remap them for compatibility
	
	if ( value <  8)
		value = value + 128;
	
	Wire.beginTransmission(g_i2caddress);
	command(value);
	Wire.endTransmission();
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Clear the display, and put cursor at 0,0 
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::clear(){
	
	command(0x0C);
	delay(g_cmdDelay);  
		
}



// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Home to custor to 0,0
// []
// []	Do not disturb data on the displayClear the display
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::home(){

	command(0x01);
	delay(g_cmdDelay);									

}





// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the display
// []
// []	Depending on the display, might just turn backlighting on
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::on(){
	
	command(0x13);
	delay(g_cmdDelay);  
}





// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the display
// []
// []	Depending on the display, might just turn backlighting off
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::off(){
	
	command(0x14);
	delay(g_cmdDelay);
	
}




// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the underline cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::cursor_on(){
	
	command(0x05);
	delay(g_cmdDelay);
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the underline cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::cursor_off(){
	
	command(0x04);
	delay(g_cmdDelay);
	
}




// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn on the blinking block cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::blink_on(){
	
	command(0x06);
	delay(g_cmdDelay);
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Turn off the blinking block cursor
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]

void LCDi2cR::blink_off(){
	
	command(0x04);
	delay(g_cmdDelay);
	
}


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Position the cursor to position line,column
// []
// []	line is 0 - Max Display lines
// []	column 0 - Max Display Width
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::setCursor(uint8_t Line, uint8_t Col){
	
	Line++;		// This display starts counting at 1, so this brings it in line with the class definitions
	Col++;
	
	Wire.beginTransmission(g_i2caddress);
	Wire.send(0x0);
	Wire.send(0x03);
	Wire.send(Line);
	Wire.send(Col);
	Wire.endTransmission();
	delay(g_cmdDelay);
	
}





// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Extended Functions
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]



#ifdef _LCDEXPANDED					


// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Adust the brightness of the backlight
// []
// []	For this display we only have on and off, so 
// []	0 will turn it off
// []   any other value will turn it on
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::backlight(uint8_t value){

	if (value == 0){
			command(0x14);
	}else{ 
			command(0x3);
	}
}



	
	// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
	// []
	// []	Return the status of the display
	// []
	// []	For this display returns the number of free bytes
	// []	in the FIFO (Returns 0-64)
	// []
	// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]	
	
int LCDi2cR::status(){
	
	int DisplayStatus = 0;
	
	//  Connect to device and request byte
	Wire.beginTransmission(g_i2caddress);
	Wire.requestFrom(g_i2caddress, 1);
	
	if (Wire.available()) {
		DisplayStatus = Wire.receive();
	}
	
	
	return DisplayStatus;
	
	
}


	
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Read data from keypad
// []
// []	
// []	
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


uint8_t LCDi2cR::keypad (){
	
  uint8_t reg0 = 0;
  uint8_t reg1 = 0;
  uint8_t reg2 = 0;
 
 
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, 3);			// Read 3 bytes 
 
  reg0 = Wire.receive();						// FIFO, just ignore
  reg1 = Wire.receive();						// Keypad state Low byte
  reg2 = Wire.receive();						// Keypad state High byte
  Wire.endTransmission();
	
 
	if (reg1 & 1) return 1;
	if (reg1 & 2) return 2;
	if (reg1 & 4) return 3;
	if (reg1 & 8) return 5;
	if (reg1 & 16) return 6;
	if (reg1 & 32) return 7;
	if (reg1 & 64) return 9;
	if (reg1 & 128) return 10;
	if (reg2 & 1) return 11;
	if (reg2 & 2) return 13;
	if (reg2 & 4) return 14;
	if (reg2 & 8) return 15;
 
 return 0;
 
}

// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]
// []
// []	Load data for a custom character
// []
// []	Char = custom character number 0-7
// []	Row is array of chars containing bytes 0-7
// []
// []
// [][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]


void LCDi2cR::load_custom_character(unsigned char char_num, unsigned char *rows)
{
	
	
	Wire.beginTransmission(g_i2caddress);
	Wire.send(0x00);
	Wire.send(0x1B);
	Wire.send(char_num+128);
	
			  
	for (uint8_t i = 0; i < 8; i++)
		Wire.send(rows[i]+128);
	Wire.endTransmission();
}






#endif



