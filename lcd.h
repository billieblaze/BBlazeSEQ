
char* lcdData[4][4];

void setupLCD(){
  lcd.init();       
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");
  delay(500);
  lcd.clear();
}

void updateLCDArray(){
   // row 1
  lcdData[0][0] = "Chan ";
  lcdData[0][1] = "Step ";
  lcdData[0][2] = "Mode ";
  lcdData[0][3] = "Ptch ";
    
  // row2 
  lcdData[1][0] += currentChannel+1;
  lcdData[1][1] += currentStep;
 
  if (editMode == 0){  lcdData[1][2] = " Play ";  }
  if (editMode == 1){  lcdData[1][2] = " Edit ";  }
  if (editMode == 2){  lcdData[1][2] = " Rec  ";  }

  lcdData[1][3] += patternData[currentChannel][3][0];
  
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

  lcdData[3][1] += patternData[currentChannel][1][currentStep] ;    
  lcdData[3][2] += patternData[currentChannel][2][currentStep];
  lcdData[3][3] = "";     
 }


void writeToLCD(){
  
  lcd.setCursor(0,0);   

  lcd.print(lcdData[0][0]);  
  lcd.print(lcdData[0][1]);
  lcd.print(lcdData[0][2]);
  lcd.print(lcdData[0][3]);

  lcd.setCursor(0,1);
  
  lcd.print(lcdData[1][0]);
  lcd.print(lcdData[1][1]);
  lcd.print(lcdData[1][2]);
  lcd.print(lcdData[1][3]);
  
/*  lcd.setCursor(0,2);    
  
  lcd.print(lcdData[2][0]);
  lcd.print(lcdData[2][1]);
  lcd.print(lcdData[2][2]);
  lcd.print(lcdData[2][3]);
  
  lcd.setCursor(0,3);   
  
  lcd.print(lcdData[3][0]);
  lcd.print(lcdData[3][1]);
  lcd.print(lcdData[3][2]);
  lcd.print(lcdData[3][3]);*/
};


static WORKING_AREA(waLCD, 64);

static msg_t doLCD(void *arg) {
 Serial.println("Scan LCD");
  chBSemSignal(&semLCD);
  while (1) {
   chBSemWait(&semLCD);
     Serial.println("Process LCD");
     
     updateLCDArray();
     writeToLCD();   

    Serial.println("LCD Updated");
  
  }
  return 0;  
}

