void initResetDetector(){

  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  
}

bool checkReset() {

  if (drd->detectDoubleReset()) {
    
  digitalWrite(LED_BUILTIN, HIGH);
  #ifdef pantallaLCD
    printLCD(0, 0, "----  Modo  ----", 1, 0, " Configuracion");
  #endif
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.startConfigPortal("Cervecero_Machine");
  digitalWrite(LED_BUILTIN, LOW);
  } else {

    digitalWrite(LED_BUILTIN, LOW);

    printLCD(0, 0, "Start in", 1, 0, ""); 

    for (int i = SECONDSTOENTER; i > 0; i--) {
      
      lcd.setCursor(9, 0);
      lcd.print(i);
      delay(1000);

    }

    Serial.println("No Double Reset Detected");
  }

  drd->stop();
}
