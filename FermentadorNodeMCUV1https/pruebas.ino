

void pruebas(){
  int menu;
  int dato1;
  int dato2;
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
        //Serial.println(datosString);
        http.end();
        
        const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
        DynamicJsonDocument doc(capacity);

        //const char* json = "{\"menu\":\"1\",\"dato1\":\"1\",\"dato2\":\"1\"}";
        
        deserializeJson(doc, datos);

        menu = doc["menu"];
        dato1 = doc["dato1"];
        dato2 = doc["dato2"];
        
  
        //Serial.println(dato);
        if (menu != 0){
          String consulta = host + "json.php?reset=1&IDplaca=";
          consulta = consulta + IDplaca;
          http.begin(*client, consulta);
          http.GET();
          http.end();
          Serial.println(menu);
          Serial.println(dato1);
          Serial.println(dato2);
          break;
        }
        }else{
          Serial.println("El servidor no responde");
        }
        
      }
   }
//Despues del while
menuPruebas(menu,dato1,dato2);

}

void menuPruebas(int menu, int dato1, int dato2){
  Serial.println("menuPruebas");
       if (menu==1) { IDreceta = dato1; leerReceta();}
  else if (menu==2) { lanzar_Procesos(dato1,dato2);}
  else if (menu==4) { trasvase();}
  
  else Serial.println("La accion deseada no existe-> menuPruebas");
}

void lanzar_Procesos(int proceso, int paso){
       if (proceso==1) { pasoProceso = paso; maceracion();}
  else if (proceso==2) { pasoProceso = paso; coccion();}
  else if (proceso==3) { pasoProceso = paso; fermentacion();}
  else Serial.println("La accion deseada no existe-> lanzar_Procesos");
}
