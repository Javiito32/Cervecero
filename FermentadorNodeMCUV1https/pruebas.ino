int menu;
int datoN;

void pruebas(){
  while(true){
  delay(100);
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = host + "json.php?IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datosString = http.getString();
        const char * datos = datosString.c_str();
        Serial.println(datosString);
        Serial.println(datos);
        http.end();
        
        const size_t capacity = JSON_OBJECT_SIZE(2) + 20;
        DynamicJsonDocument doc(capacity);

        //const char* json = "{\"menu\":\"1\",\"dato\":\"1\"}";
        
        deserializeJson(doc, datos);

        menu = doc["menu"];
        datoN = doc["dato"];
        Serial.println(menu);
        Serial.println(datoN);
  
        //Serial.println(dato);
        if (menu != 0 && datoN != 0){
          String consulta = host + "json.php?reset=1&IDplaca=";
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
//Despues del while
menuPruebas(menu,datoN);

}

void menuPruebas(int menu, int dato){
  Serial.println("menuPruebas");
       if (menu==1) { IDreceta = dato; leerReceta();}
  else if (menu==2) { pasoProceso = dato; maceracion();}
  else if (menu==3) { pasoProceso = dato; coccion();}
  else if (menu==4) { trasvase();}
  else if (menu==5) { pasoProceso = dato; fermentacion();}
  
  else Serial.println("La accion deseada no existe");
}
