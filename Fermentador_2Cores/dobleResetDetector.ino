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
  wifiManager.startConfigPortal("Cervecero_2.0");
  digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    #ifdef debug
      Serial.println("No Double Reset Detected");
    #endif
  }
  delay(5000);
  drd->stop();
  
}
