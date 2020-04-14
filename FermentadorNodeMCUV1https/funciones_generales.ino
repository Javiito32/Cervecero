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




void sendInfo(int proceso,byte pasoProceso) {
  Serial.println("Log");
  if (WiFi.status() == WL_CONNECTED) {
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&receta=");
    datos_Enviar.concat(IDreceta);
    datos_Enviar.concat("&proceso=");
    datos_Enviar.concat(proceso);
    datos_Enviar.concat("&pasoProceso=");
    datos_Enviar.concat(pasoProceso);
    datos_Enviar.concat("&estado=");
    datos_Enviar.concat(estado);
    datos_Enviar.concat("&tiempoRestante=");
    datos_Enviar.concat(tiempoRestante);
    datos_Enviar.concat("&porcentaje="); 
    datos_Enviar.concat(porcentaje);
    peticion("info.php",datos_Enviar);
  }
}

int count(String str){
  int a = 1;
  for (int i = 0; i < str.length(); i ++){
      if (str[i] == ':') {a = a + 1;}
  }
  return a;
}


String peticion(String php, String datos_Enviar){
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin(*client,host+php);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(datos_Enviar);
    if (httpCode == 200 || httpCode == 201) {
      String datos = http.getString();
      http.end();
      return datos;
    }else{
      http.end();
      return "fallo";
    }
}
