
 unsigned long LCDLastUpdated=0;
// LCD - using a sainSmart i2c LCD connected to 20/21 of my MEGA
  LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
  
  char* lcdData[4][4];
  char* lcdData_old[4][4];
  
     char rowText1[255]; 
   char rowText2[255]; 
   char rowText3[255]; 
   char rowText4[255]; 

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
    lcdData[0][0] = "Chan ";
    lcdData[0][1] = "Step ";
    lcdData[0][2] = "Mode ";
    lcdData[0][3] = "Ptch ";
    
   
    // row 2
   sprintf(buffer1, " %d   ", currentChannel+1);
   lcdData[1][0] = buffer1;
   
   
   sprintf(buffer2, " %d  ", currentStep);
   lcdData[1][1] = buffer2;
 
  
    if (editMode == 0){
     lcdData[1][2] = " Play ";
    }
  
    if (editMode == 1){
       lcdData[1][2] = " Edit ";
    }
  
    if (editMode == 2){
       lcdData[1][2] = " Rec  ";
    }
    

  sprintf(buffer3, " %d ", patternData[currentChannel][3][0]);
   lcdData[1][3] = buffer3;
   
    // row 3
    lcdData[2][0] = "Note ";
    lcdData[2][1] = "NNum ";
    lcdData[2][2] = "Velo ";
    lcdData[2][3] = "     ";
  
    // row 4
    int noteOnData = patternData[currentChannel][0][currentStep];
    switch(noteOnData){
      case 0:
        lcdData[3][0] = " off ";
        break;
      case 1:
        lcdData[3][0] = " on  ";
        break;
      case 2:
        lcdData[3][0] = " hld ";
        break;
    }
     sprintf(buffer4, " %d ", patternData[currentChannel][1][currentStep]);
   lcdData[3][1] = buffer4 ; 
    sprintf(buffer5, " %d ",patternData[currentChannel][2][currentStep]);
   lcdData[3][2] = buffer5;
   lcdData[3][3] = "";
   
     sprintf(rowText1, "%s%s%s%s",lcdData[0][0], lcdData[0][1], lcdData[0][2],lcdData[0][3]);
     sprintf(rowText2, "%s%s%s%s",lcdData[1][0], lcdData[1][1], lcdData[1][2],lcdData[1][3]);
     sprintf(rowText3, "%s%s%s%s",lcdData[2][0], lcdData[2][1], lcdData[2][2],lcdData[2][3]);
     sprintf(rowText4, "%s%s%s%s",lcdData[3][0], lcdData[3][1], lcdData[3][2],lcdData[3][3]);
  updateLCD = 2;
 }
}

void writeToLCD(){


   if(startTime - LCDLastUpdated > 1000 && lcdData != lcdData_old && updateLCD == 2){  


        
     lcd.setCursor(0,0);
        
        
        lcd.print(rowText1);
        lcd.setCursor(0,1);
        
        lcd.print(rowText2);
        lcd.setCursor(0,2);

        lcd.print(rowText3);
        lcd.setCursor(0,3);

        lcd.print(rowText4);
        
        
     for ( int i = 0; i < 4; i++){ 
      
       lcdData_old[i][0] = lcdData[i][0];
       lcdData_old[i][1] = lcdData[i][1];
       lcdData_old[i][2] = lcdData[i][2];
       lcdData_old[i][3] = lcdData[i][3];
    }
   
    LCDLastUpdated = millis();   
  }
 updateLCD = 0;

};
