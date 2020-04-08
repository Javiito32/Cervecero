void pregunta(){
  while(true){
  delay(100);
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = host + "menu.php?menu=1&IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datoString = http.getString();
        http.end();
        dato = datoString.toInt();
        //Serial.println(dato);
        if (dato != 0){
          String consulta = host + "menu.php?resetmenu=1&IDplaca=";
          consulta = consulta + IDplaca;
          http.begin(*client, consulta);
          http.GET();
          http.end();
          break;
        }
        }else{
          Serial.println("El servidor no responde");
        }
        
      }
  }
}


/*
 * Metodo para enviar el final del proceso con errores.
 * Envia un mensaje a la Rasberry con los errores del proceso.
 * 
 * Parametros: dato  Representa el numero de proceso
 *             error Representa el numero de error (0 si no hay)
 * No devuelve nada
 */
void finProceso (int proceso,bool error){
//Variables locales
  String mensaje = "Proceso ";
//Conversion a String
  mensaje.concat(proceso);
  mensaje.concat(" Fallo ");
  mensaje.concat(error);

  if (falloProceso){
    if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = host + "resetCancelar.php?IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client, consulta);
    http.GET();
    http.end();
    falloProceso = 0;
  }
  }
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}

void sendInfo(int proceso,byte pasoProceso) {
  Serial.println("Log");
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String peticion = host + "info.php?IDplaca=";
    peticion = peticion + IDplaca;
    peticion = peticion + "&receta=";
    peticion = peticion + IDreceta;
    peticion = peticion + "&proceso=";
    peticion = peticion + proceso;
    peticion = peticion + "&pasoProceso=";
    peticion = peticion + pasoProceso;
    peticion = peticion + "&estado=";
    peticion = peticion + estado;
    peticion = peticion + "&tiempoRestante=";
    peticion = peticion + tiempoRestante;
    peticion = peticion + "&porcentaje=";
    peticion = peticion + porcentaje;
    http.begin(*client, peticion);
    Serial.println(peticion);
    http.GET();
    http.end();
  }
}

int count(String str){
  int a = 1;
  for (int i = 0; i < str.length(); i ++){
      if (str[i] == ':') {a = a + 1;}
  }
  return a;
}



void comprobarCancelar() {
  if (WiFi.status() == WL_CONNECTED){
    //Serial.println("Comprobación de cancelación");
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String datos_enviar = "?IDplaca=" + IDplaca;
    //Serial.println(consulta);
    http.begin(*client,host + "checkCancel.php");  // Request destination.
    int httpCode = http.POST(datos_enviar); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String stringcancelar = http.getString();
        int cancelar = stringcancelar.toInt();
        if (cancelar == 1){
          falloProceso = 1;
          http.end();
        }
    }else{
      Serial.println("No se pudo comprobar la cancelación del proceso");
    }
   }
}
