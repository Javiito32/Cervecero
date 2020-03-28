void ClientRequest(){
  //client->setFingerprint(fingerprint);
  
  consulta = server + consulta;
  //Serial.println(consulta);
  http.begin(*client, consulta);  // Request destination.
  int httpCode = http.GET(); // Send the request.
  if (httpCode == 200 || httpCode == 201) {
    String dato = http.getString();
    http.end();
  }else{
      Serial.println("------------------------------");
      Serial.println("No se pudo obtener el ID de placa");
      Serial.println("------------------------------"); 
  }
}
