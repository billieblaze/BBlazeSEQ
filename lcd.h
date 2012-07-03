void setupLCD(){
  lcd.init();                   
  lcd.backlight();
  lcd.print("bblazeSEQ v" + version);
  lcd.setCursor(0,1);
  lcd.print("Init...");
}

void handleLCD(){
 lcd.home();
  lcd.print("CH");
  lcd.print(currentChannel+1);
  lcd.print(" - Step " +currentStep);
  lcd.print(" ");

  if (editMode == 0){
    lcd.print("PLAY");
  }

  if (editMode == 1){
    lcd.print("EDIT");
  }

  if (editMode == 2){
    lcd.print("REC ");
  }

  lcd.setCursor(0,1);
  lcd.print("ON:");
  if(editParam == 0){
    lcd.print(">");
  }


  int noteOnData = patternData[currentChannel][0][currentStep];
  switch(noteOnData){
  case 0:
    lcd.print("off");
    break;
  case 1:
    lcd.print("on");
    break;
  case 2:
    lcd.print("hld");
    break;

  }

  lcd.print(" N#:");
  if(editParam == 1){
    lcd.print(">");
  }
  lcd.print(patternData[currentChannel][1][currentStep]);

  lcd.print(" V:");
  if(editParam == 2){
    lcd.print(">");
  }
  lcd.print(patternData[currentChannel][2][currentStep]);
  lcd.print(" ");

  lcd.setCursor(0,2);

  if(editParam == 3){
    lcd.print(">");
  }
  lcd.print("Patch ");
  lcd.print(patternData[currentChannel][3][0]);
}

