void sendInfo(int proceso,byte pasoProceso) {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String peticion = "https://192.168.1.150/arduino/info.php?IDplaca=";
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
    //Serial.println(peticion);
    http.GET();
    http.end();
  }
}
