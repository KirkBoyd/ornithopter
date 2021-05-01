void printSticksSerial(){
  //print joystick A info
  Serial.print("\nAx: ");
  Serial.print(data_robomote.Ax);
  Serial.print("\t\t Ay: ");
  Serial.print(data_robomote.Ay);
  Serial.print("\t\t Az: ");
  Serial.print(data_robomote.Az);
  Serial.print("\t\t AT: ");
  Serial.print(data_robomote.Atrig);
  //print joystick B info
  Serial.print("\t\t Bx: ");
  Serial.print(data_robomote.Bx);
  Serial.print("\t\t By: ");
  Serial.print(data_robomote.By);
  Serial.print("\t\t Bz: ");
  Serial.print(data_robomote.Bz);
  Serial.print("\t\t BT: ");
  Serial.print(data_robomote.Btrig);
}

void printData(){
  Serial.print("\nHall1: ");
  Serial.print(data_birdo.hall1);
  Serial.print("\t\t Hall2: ");
  Serial.print(data_birdo.hall2);
  Serial.print("\t\t Ax: ");
  Serial.print(data_birdo.accelX);
  Serial.print("\t\t Ay: ");
  Serial.print(data_birdo.accelY);
}
