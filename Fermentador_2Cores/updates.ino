bool checkforUpdates(){
  while(true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&currentVersion=" + currentVersion);
    String datos = peticion("checkforUpdates.php",datos_Enviar);
    //Serial.println(datos_Enviar);
      if (datos != "fallo") {
        int n = s.separa(datos, ':', 0).toInt();
        int u = s.separa(datos, ':', 1).toInt();
        #ifdef debug
          Serial.println("---------------------");
          Serial.println(datos);
          Serial.println(n);
          Serial.println(u);
        #endif
        if (n == 1){
          #ifdef pantallaLCD
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Actualizacion");
          lcd.setCursor(0,1);
          lcd.print("---Disponible---");
          #endif
          #ifdef debug
            Serial.println("Actualización disponible");
          #endif
          delay(5000);
          if (u == 1){
            #ifdef pantallaLCD
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Actualizando");
            lcd.setCursor(0,1);
            lcd.print("---No apagar---");
            #endif
            #ifdef pantallaLCD
            Serial.println("Comenzando proceso de actualización");
            Serial.println("-----------------------------------");
            #endif
            //ESPhttpUpdate.update(updatesServer, 80, "/firmware/arduino.bin");
            WiFiClient client;
            httpUpdate.update(client, updatesServer, 80, "/firmware/arduino.bin");
          }
        }else{
          #ifdef pantallaLCD
            Serial.println("No hay actualizaciones disponibles");
          #endif
        }
        break;
      }else{
        #ifdef pantallaLCD
          Serial.println("------------------------------");
          Serial.println("Error al comprobar actualizaciones");
          Serial.println("------------------------------");
        #endif
        #ifdef pantallaLCD
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Error al");
          lcd.setCursor(0,1);
          lcd.print("check updates");
        #endif
      }
  }
}
