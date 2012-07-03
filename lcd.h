void setupLCD(){
  lcd.init();                   
  lcd.backlight();
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");
}

void handleLCD(){
 if (updateLCD == 1){
  // row 1
  lcd.setCursor(0,0);
  lcd.print("Chan ");
  lcd.print("Step ");
  lcd.print("Mode ");
  lcd.print("Ptch ");
  
  
  // row 2
  
  lcd.setCursor(0,1);
  lcd.print(currentChannel+1);
  lcd.print("     ");
  
  lcd.print(currentStep);
  lcd.print("   ");

  if (editMode == 0){
    lcd.print("Play  ");
  }

  if (editMode == 1){
    lcd.print("Edit  ");
  }

  if (editMode == 2){
    lcd.print("Rec   ");
  }
  lcd.print(patternData[currentChannel][3][0]);
  lcd.print("  ");

  // row 3
  lcd.setCursor(0,2);
 
  lcd.print("Note ");
  lcd.print("NNum ");
  lcd.print("Velo ");
  lcd.print("     ");
  lcd.setCursor(0,3);
  int noteOnData = patternData[currentChannel][0][currentStep];
  switch(noteOnData){
    case 0:
      lcd.print("off   ");
      break;
    case 1:
      lcd.print("on    ");
      break;
    case 2:
      lcd.print("hld   ");
      break;
  }

  lcd.print(patternData[currentChannel][1][currentStep]);
  lcd.print("   ");

  lcd.print(patternData[currentChannel][2][currentStep]);
  lcd.print(" ");
  updateLCD = 0;
 }
}

