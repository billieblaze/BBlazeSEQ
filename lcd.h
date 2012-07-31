// I had to increase the freq of i2c in twi.h (inside wire library) to 300hz.  i also commented out the delays in there to try and tighten up the response

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
char buffer1[255];
char buffer2[255];
char buffer3[255];
char buffer4[255];
char buffer5[255];

   if(updateLCD == 1){

    // row 1
    lcdData[0][0] = "Chan";
    lcdData[0][1] = "Step";
    lcdData[0][2] = "Mode";
    lcdData[0][3] = "Ptch";
    
   
    // row 2
   sprintf(buffer1, "%d", currentChannel+1);
   lcdData[1][0] = buffer1;
   
   
   sprintf(buffer2, "%d", currentStep);
   lcdData[1][1] = buffer2;
 
  
    if (editMode == 0){
     lcdData[1][2] = "Play";
    }
  
    if (editMode == 1){
       lcdData[1][2] = "Edit";
    }
  
    if (editMode == 2){
       lcdData[1][2] = "Rec ";
    }
    

  sprintf(buffer3, "%d", patternData[currentChannel][3][0]);
   lcdData[1][3] = buffer3;
   
    // row 3
    lcdData[2][0] = "Note";
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
     sprintf(buffer4, "%d", patternData[currentChannel][1][currentStep]);
   lcdData[3][1] = buffer4;
    sprintf(buffer5, "%d",patternData[currentChannel][2][currentStep]);
   lcdData[3][2] = buffer5;
   lcdData[3][3] = "";

  updateLCD = 0;
 }
}

void writeToLCD(){
  
    for ( int i = 0; i < 4; i++){ 
      if(lcdData[i] != lcdData_old[i]){   

       lcd.setCursor(0,i);
       
       lcd.print(lcdData[i][0]);
       lcd.setCursor(5,i);
       lcd.print(lcdData[i][1]);
       lcd.setCursor(10,i);
       lcd.print(lcdData[i][2]);
       lcd.setCursor(15,i);
       lcd.print(lcdData[i][3]);
          
       lcdData_old[i][0] = lcdData[i][0];
       lcdData_old[i][1] = lcdData[i][1];
       lcdData_old[i][2] = lcdData[i][2];
       lcdData_old[i][3] = lcdData[i][3];
    }
  }
  LCDLastUpdated = millis();
};
