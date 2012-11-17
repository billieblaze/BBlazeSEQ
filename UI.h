void scanUI( ){
  byte bitVal;
  byte bytesVal;

    digitalWriteFast(navploadPin, LOW);
    digitalWriteFast(navploadPin, HIGH);
  
    for ( int row = 0; row < 4; row++){
      bytesVal = 0;
      
      for(int i = 0; i < 8; i++){
        bitVal = PINA & ( 1 << PA0 );
       // bitVal = digitalReadFast(navdataPin);
        bytesVal |= (bitVal  << (7 - i));

        digitalWriteFast(navclockPin, HIGH);
        digitalWriteFast(navclockPin, LOW);
      } 
  
      newNavKeyValues[row]=bytesVal;
      
       MIDI.read();
       
    }
  scanKeypad();
}


