bool checkforUpdates(){
  while(true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&currentVersion=" + currentVersion);
    String datos = peticion("checkforUpdates.php",datos_Enviar);
    //Serial.println(datos_Enviar);
      if (datos == "fallo") {

        #ifdef pantallaLCD
          Serial.println("------------------------------");
          Serial.println("Error al comprobar actualizaciones");
          Serial.println("------------------------------");
        #endif
        #ifdef pantallaLCD
          printLCD(0, 0, " Error checking", 1, 0, "   updates");
        #endif

      }else{

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
            printLCD(0, 0, " Actualizacion", 1, 0, "---Disponible---");
          #endif
          #ifdef debug
            Serial.println("Actualización disponible");
          #endif
          delay(5000);
          if (u == 1){
            #ifdef pantallaLCD
              printLCD(0, 0, " Actualizando", 1, 0, "---No apagar---");
            #endif
            #ifdef pantallaLCD
            Serial.println("Comenzando proceso de actualización");
            Serial.println("-----------------------------------");
            #endif
            
            t_httpUpdate_return ret = httpUpdate.update(wifiClient, ipServer, 80, "/firmware/arduino.bin");

            switch (ret) {
              case HTTP_UPDATE_FAILED:
                #ifdef debug
                  Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
                #endif
                break;

              case HTTP_UPDATE_NO_UPDATES:
                #ifdef debug
                  Serial.println("HTTP_UPDATE_NO_UPDATES");
                #endif
                break;

              case HTTP_UPDATE_OK:
                #ifdef debug
                  Serial.println("HTTP_UPDATE_OK");
                #endif
                break;
    }
          }
        }else{
          #ifdef pantallaLCD
            Serial.println("No hay actualizaciones disponibles");
          #endif
        }
        break;
      }
  }
}
