#include <Wire.h> //Include the Arduino I2C Library
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
int rdata = data;

Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); // MSB
Wire.write((int)(eeaddress & 0xFF)); // LSB

Wire.write(rdata);

Wire.endTransmission();
}

void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddresspage >> 8)); // MSB
Wire.write((int)(eeaddresspage & 0xFF)); // LSB

byte c;
for ( c = 0; c < length; c++)
Wire.write(data[c]);

Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
byte rdata = 0xFF;

Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); // MSB
Wire.write((int)(eeaddress & 0xFF)); // LSB
Wire.endTransmission();
Wire.requestFrom(deviceaddress,1);

if (Wire.available()) rdata = Wire.read();

return rdata;
}

void setup() {
char somedata[] = "dayzlab.wordpress.com"; //Data
Wire.begin(); //Start I2C connections
Serial.begin(9600);
i2c_eeprom_write_page(0x50, 0, (byte *)somedata, sizeof(somedata)); // write to EEPROM

delay(10); //add a small delay

Serial.println("Memory written");
}

void loop() {
int addr=0; //EEPROM Address 0
byte b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory

while (b!=0) {
Serial.print((char)b); //print content to serial port
addr++; //increase address
b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
}

Serial.println(" ");
delay(2000);
}
