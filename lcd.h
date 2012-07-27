void setupLCD(){
  lcd.init();                   
  lcd.backlight();
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");
  delay(500);
  lcd.clear();
}

void updateLCDArray(){

   if(updateLCD == 1){

    // row 1
    lcdData[0][0] = "Chan";
    lcdData[0][1] = "Step";
    lcdData[0][2] = "Mode";
    lcdData[0][3] = "Ptch";
    
   
    // row 2
    itoa(currentChannel+1, lcdData[1][0], 10);
    itoa(currentStep, lcdData[1][1], 10);
  
    if (editMode == 0){
     lcdData[1][2] = "Play";
    }
  
    if (editMode == 1){
       lcdData[1][2] = "Edit";
    }
  
    if (editMode == 2){
       lcdData[1][2] = "Rec ";
    }
    
    itoa(patternData[currentChannel][3][0],lcdData[1][3], 10);
  
    // row 3
    lcdData[2][0] = "NNum";
    lcdData[2][1] = "NNum";
    lcdData[2][2] = "Velo";
    lcdData[2][3] = "    ";
  
    // row 4
    int noteOnData = patternData[currentChannel][0][currentStep];
    switch(noteOnData){
      case 0:
        lcdData[3][0] = "off ";
        break;
      case 1:
        lcdData[3][0] = "on  ";
        break;
      case 2:
        lcdData[3][0] = "hld ";
        break;
    }
  
  itoa(patternData[currentChannel][1][currentStep], lcdData[3][1], 10);
  itoa(patternData[currentChannel][2][currentStep], lcdData[3][2], 10);
  
  updateLCD = 0;
 }
}

void writeToLCD(){
 if(lcdData != lcdData_old){    
    for ( int i = 0; i < 4; i++){ 
      
       StringSumHelper printIt=       String(lcdData[i][0]) + " " +       String(lcdData[i][1]) + " " +       String(lcdData[i][2]) + " " +       String(lcdData[i][3]) + " ";
       lcd.setCursor(0,i);
       lcd.print(printIt);
       lcdData_old[1][0] = lcdData[1][0];
       lcdData_old[1][1] = lcdData[1][1];
       lcdData_old[1][2] = lcdData[1][2];
       lcdData_old[1][3] = lcdData[1][3];
    }
  }
  LCDLastUpdated = millis();
};
