void printSticksLcd(){
  //print joystick A info
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ax");
  lcd.print(analogRead(joyAx));
  lcd.print("y");
  lcd.print(analogRead(joyAy));
  lcd.print("z");
  lcd.print(analogRead(joyAz));
  lcd.print("T");
  lcd.print(digitalRead(joyTrigA));
  //print joystick B info
  lcd.setCursor(0,1);
  lcd.print("Bx");
  lcd.print(analogRead(joyBx));
  lcd.print("y");
  lcd.print(analogRead(joyBy));
  lcd.print("z");
  lcd.print(analogRead(joyBz));
  lcd.print("T");
  lcd.print(digitalRead(joyTrigB));
  delay(200);
}

void printSticksSerial(){
  //print joystick A info
  Serial.print("\nAx: ");
  Serial.print(analogRead(joyAx));
  Serial.print("\t Ay: ");
  Serial.print(analogRead(joyAy));
  Serial.print("\t Az: ");
  Serial.print(analogRead(joyAz));
  Serial.print("\t AT: ");
  Serial.print(digitalRead(joyTrigA));
  //print joystick B info
  Serial.print("\t Bx: ");
  Serial.print(analogRead(joyBx));
  Serial.print("\t By: ");
  Serial.print(analogRead(joyBy));
  Serial.print("\t Bz: ");
  Serial.print(analogRead(joyBz));
  Serial.print("\t BT: ");
  Serial.print(digitalRead(joyTrigB));
}
