bool checkforUpdates(){

  String data_To_Send = "id_Placa=";
  data_To_Send.concat(id_Board);
  data_To_Send.concat("&currentVersion=" + currentVersion);
  String datos = peticion("checkforUpdates.php", data_To_Send);

    if (datos == "fallo") {

      #ifdef pantallaLCD
        Serial.println("------------------------------");
        Serial.println("Error al comprobar actualizaciones");
        Serial.println("------------------------------");
      #endif
      #ifdef pantallaLCD
        printLCD(0, 0, " Error checking", 1, 0, "   updates");
        delay(1000);
      #endif

    } else{

      const size_t capacity = JSON_OBJECT_SIZE(2) + 40;
      DynamicJsonDocument doc(capacity);

      const char* json = datos.c_str();
      Serial.println(datos);

      deserializeJson(doc, json);

      int updateAvailable = doc["updateAvailable"];
      int updateNow = doc["updateNow"];

      Serial.println(updateAvailable);
      Serial.println(updateNow);

      if (updateAvailable){

        #ifdef pantallaLCD
          printLCD(0, 0, " Actualizacion", 1, 0, "---Disponible---");
        #endif

        Serial.println("Actualización disponible");
        delay(5000);

        if (updateNow){
          
          #ifdef pantallaLCD
            printLCD(0, 0, " Actualizando", 1, 0, "---No apagar---");
          #endif

          Serial.println("Comenzando proceso de actualización");
          Serial.println("-----------------------------------");
          
          t_httpUpdate_return ret = httpUpdate.update(wifiClient, ipServer, 80, "/firmware/arduino.bin");

          switch (ret) {
            case HTTP_UPDATE_FAILED:

              Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
              break;

            case HTTP_UPDATE_NO_UPDATES:
            
              Serial.println("HTTP_UPDATE_NO_UPDATES");
              break;

            case HTTP_UPDATE_OK:

              Serial.println("HTTP_UPDATE_OK");
              break;
          }
        }
      }else Serial.println("No hay actualizaciones disponibles");
    }
}
