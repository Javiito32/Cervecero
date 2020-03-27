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
    String consulta = "https://192.168.1.150/arduino/menu.php?resetfallo=1&IDplaca=";
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
