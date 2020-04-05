void comprobarCancelar() {
  if (WiFi.status() == WL_CONNECTED){
    //Serial.println("Comprobación de cancelación");
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = host + "menu.php?fallo=1&IDplaca=";
    consulta = consulta + IDplaca;
    //Serial.println(consulta);
    http.begin(*client,consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
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
