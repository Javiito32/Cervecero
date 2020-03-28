//Solicitud de Identificador de placa según la mac
void getID(){
  while (true){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/get_id.php?mac=";
    consulta = consulta + mac;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String stringIDplaca = http.getString();
        http.end();
        IDplaca = stringIDplaca.toInt();
        Serial.println("------------------------------");
        Serial.print("El ID de la placa es el: ");
        Serial.println(IDplaca);
        Serial.println("------------------------------");
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No se pudo obtener el ID de placa");
        Serial.println("------------------------------");
      }
  }
}

//Revisión de si hay algún procesos que recuperar que se ha quedado a medias
void checkrecovery(){
  while (true){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/recovery.php?IDplaca=";
    consulta = consulta + IDplaca;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datos = http.getString();
        //Serial.println(datos);
        http.end();
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
        
     
        }break;
      }else{
        Serial.println("------------------------------");
        Serial.println("Error de conexión con el servidor");
        Serial.println("------------------------------");
        break;
      }
  }
}

bool checkforUpdates(){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/checkforUpdates.php?IDplaca=";
    consulta = consulta + IDplaca;
    consulta = consulta + "&currentVersion=";
    consulta = consulta + currentVersion;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datos = http.getString();
        //Serial.println(datos);
        http.end();
        int n = datos.toInt();
        if (n == 1){
          Serial.println("Actualización disponible");
        }else{
          Serial.println("No hay actualizaciones disponibles");
        }
        return n;
      }
}

void startUpdate(){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/startUpdate.php?IDplaca=";
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
          Serial.println("Comenzando proceso de actualización");
          ESPhttpUpdate.update("192.168.1.150", 80, "/firmware/arduino.bin");
          Serial.println("-----------------------------------");
        }
      }
  
}
