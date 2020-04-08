void inicio(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cervecero v" + currentVersion);
  lcd.setCursor(0,1);
  lcd.print("     Ready");
  //Mensaje inicial
  Serial.println("------------------------------");
  Serial.println("Ready");
  Serial.println("------------------------------");
  pruebas();
  
  //pregunta();
  //menuinicio(dato);
}
