//Solicitud de Identificador de placa según la mac
void getID(){
  while (true){
    String datos = peticion("get_id.php","mac=" + mac);
      if (datos != "fallo") {
        //Serial.println(stringIDplaca);
        IDplaca = datos.toInt();
        Serial.println("------------------------------");
        Serial.print("El ID de la placa es el: ");
        Serial.println(IDplaca);
        Serial.println("------------------------------");
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No se pudo obtener el ID de placa");
        Serial.println("------------------------------");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el ID de placa");
      }
  }
}

//Revisión de si hay algún procesos que recuperar que se ha quedado a medias
void checkrecovery(){
  Serial.println("Comprobando Recovery");
  while (true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    String datos = peticion("recovery.php",datos_Enviar);
      if (datos != "fallo") {
        //Serial.println(datos);
        
        //Decode
        int longitud = datos.length();
        int pestado = datos.indexOf("estado=");
        String sestado = "";
        for (int i = pestado + 7; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else sestado += datos[i];
        }
        int estado = sestado.toInt();

        if(estado == 1){
          recovery = 1;
          Serial.println("------------------------------");
          Serial.println("Hay procesos pendiantes");
          Serial.println("------------------------------");


          int preceta = datos.indexOf("receta=");
          String sreceta = "";
          for (int i = preceta + 7; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sreceta += datos[i];
          }
          IDreceta = sreceta.toInt();
          

          int ptiempoRestante = datos.indexOf("tiempoRestante=");
          String stiempoRestante = "";
          for (int i = ptiempoRestante + 15; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else stiempoRestante += datos[i];
          }
          recoveryTiempoRestante = stiempoRestante.toInt();



          int pproceso = datos.indexOf("proceso=");
          String sproceso = "";
          for (int i = pproceso + 8; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sproceso += datos[i];
          }
          recoveryProceso = sproceso.toInt();



          int ppasoProceso = datos.indexOf("pasoProceso=");
          String spasoProceso = "";
          for (int i = ppasoProceso + 12; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else spasoProceso += datos[i];
          }
          recoveryPasoProceso = spasoProceso.toInt();

        Serial.print("Receta a recuperar: ");
        Serial.println(IDreceta);
        Serial.print("Tiempo que le falta: ");
        Serial.println(recoveryTiempoRestante);
        Serial.print("Proceso que estaba: ");
        Serial.println(recoveryProceso);
        Serial.print("Paso del proceso que estaba: ");
        Serial.println(recoveryPasoProceso);
        break;
     
        }else{break;}
      }else{
        Serial.println("------------------------------");
        Serial.println("Error de conexión con el servidor");
        Serial.println("------------------------------");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el Recovery");
      }
  }
}

bool checkforUpdates(){
  while(true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&currentVersion=" + currentVersion);
    String datos = peticion("checkforUpdates.php",datos_Enviar);
    Serial.println(datos_Enviar);
      if (datos != "fallo") {
        Serial.println(datos);
        int n = s.separa(datos, ':', 0).toInt();
        Serial.println(n);
        int u = s.separa(datos, ':', 1).toInt();
        Serial.println(u);
        if (n == 1){
          if (u == 1){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" Actualizando");
            lcd.setCursor(0,1);
            lcd.print("---No apagar---");
            Serial.println("Comenzando proceso de actualización");
            //ESPhttpUpdate.update(updatesServer, 80, "/firmware/arduino.bin");
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

/*void startUpdate(){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = host + "startUpdate.php?IDplaca=";
    consulta = consulta + IDplaca;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datos = http.getString();
        //Serial.println(datos);
        http.end();
        int n = datos.toInt();
        if (n == 1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Actualizando");
          lcd.setCursor(0,1);
          lcd.print("---No apagar---");
          Serial.println("Comenzando proceso de actualización");
          ESPhttpUpdate.update(updatesServer, 80, "/firmware/arduino.bin");
          Serial.println("-----------------------------------");
        }
      }
  
}*/
