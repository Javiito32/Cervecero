bool checkforUpdates(){
  while(true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&currentVersion=" + currentVersion);
    String datos = peticion("checkforUpdates.php",datos_Enviar);
    //Serial.println(datos_Enviar);
      if (datos != "fallo") {
        Serial.println("---------------------");
        Serial.println(datos);
        int n = s.separa(datos, ':', 0).toInt();
        Serial.println(n);
        int u = s.separa(datos, ':', 1).toInt();
        Serial.println(u);
        if (n == 1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Actualizacion");
          lcd.setCursor(0,1);
          lcd.print("---Disponible---");
          delay(5000);
          if (u == 1){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Actualizando");
            lcd.setCursor(0,1);
            lcd.print("---No apagar---");
            Serial.println("Comenzando proceso de actualización");
            ESPhttpUpdate.update(updatesServer, 80, "/firmware/arduino.bin");
            Serial.println("-----------------------------------");
          }
          Serial.println("Actualización disponible");
        }else{
          Serial.println("No hay actualizaciones disponibles");
        }
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("Error al comprobar actualizaciones");
        Serial.println("------------------------------");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al");
        lcd.setCursor(0,1);
        lcd.print("check updates");
      }
  }
}
