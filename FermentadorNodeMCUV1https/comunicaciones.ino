/*
 * Hace las peticiones de datos al servidor con una consulta por el metodo POST
 */
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
/*
 * Hace el log en la BBDD con los datos del proceso
 */

void Log(int proceso,byte pasoProceso) {
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
    peticion("log.php",datos_Enviar);
    Serial.println("Log");
  }
}
