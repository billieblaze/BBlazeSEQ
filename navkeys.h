

void handleNavButtons(){
 int bitVal = 0;

  for (int row = 0; row < 4;  row++){
    
      oldNavKeyValues[row] = newNavKeyValues[row] ;   
      
      int i = 0;
      for (byte mask = B10000000; mask>0; mask >>= 1) {
        
        bitVal = 0;

        if (newNavKeyValues[row] & mask){ 
          bitVal = 1;
          int key = (8*row) + i;
          switch(key){
            case 0:
              // enc sw 5
              break;
            case 3://enc sw 1
              break;
            case 8:
              break;
            case 9:// enc sw 2
              break;
            case 12: // enc sw 7
              break;
            case 15:// enc sw2
              break;
            case 18:// enc sw 8         
              break;
            case 19: 
              break;
            case 20:
              break;
            case 23:     
              break;
            case 24:// down  
              break;
            case 25:// Select
            
              break;
            case 26:// Right
              break;
            case 27:// UP
              break;
            case 28:// Left
              break;
            case 31: // select knob sw
              break;
            }
            
          updateLCD = 1;  
          }
          i++;
      }
    }
  
}


