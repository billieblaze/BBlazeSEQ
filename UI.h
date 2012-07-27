void scanUI( ){
  byte bitVal;
  byte bytesVal;

    digitalWrite(navploadPin, LOW);
    digitalWrite(navploadPin, HIGH);
  
    for ( int row = 0; row < 4; row++){
      bytesVal = 0;
      
      for(int i = 0; i < 8; i++){
        bitVal = PINA & ( 1 << PA0 );
        bytesVal |= (bitVal  << (7 - i));
  shiftIn(
        digitalWrite(navclockPin, HIGH);
        digitalWrite(navclockPin, LOW);
      } 
  
      newNavKeyValues[row]=bytesVal;
    }
  
}

void handleUI( ) { 
    handleEncoders();
    handleNavButtons();
    handleKeypad(); 
}
