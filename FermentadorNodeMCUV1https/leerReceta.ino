/*
 * Metodo leer. 
 * Sirve para leer una cadena de caracteres que se recibe de la SQL
 * y se almacena en un String. Los datos vienen separados por ";".
 * 
 * Contiene el parametro del ID de la receta que tiene que leer de la SQL.
 * Asigna los valores oportunos a las variables de cotrol (tmperatura y tiempo de cada proceso).
 */

void leerReceta(){
  if (WiFi.status() == WL_CONNECTED) {
    
    String peticion = "https://192.168.1.150/arduino/pedirdatos.php?id=";
    peticion = peticion + IDreceta;
    Serial.println("------------------------------");
    Serial.print("Petición al servidor: ");
    Serial.println(peticion);
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin(*client,peticion);  // Request destination.
    int httpCode = http.GET(); // Send the request.
    Serial.println(httpCode);
    if (httpCode == 200 || httpCode == 201) { //Check the returning code
      String datos = http.getString(); 
      http.end();   //Close connection
      Serial.println("------------------------------");                           // Obtiene la string
      Serial.print("String recibida: ");
      Serial.println(datos);
      int longitud = datos.length();
      Serial.println("------------------------------");

    //Procesar datos nombre Receta
      int pnombre = datos.indexOf("nombre=");
      String nombre = "";
      for (int i = pnombre + 7; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else nombre += datos[i];
      }
      

    //Procesar datos de la Temperatura de Maceración
      int ptempMacer = datos.indexOf("tempMacer=");               //Posicion de temp
      String stempMacer = "";
      for (int i = ptempMacer + 10; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempMacer += datos[i];
      }
      //tempMacer = stempMacer.toFloat();
      int numParametros = count(stempMacer);
      for (int i = 0;i < numParametros;i ++){
        tempMacer[i] = s.separa(stempMacer, ':', i);
      }
      
      

    //Procesar datos tiempo la Maceración
      int ptiempoMacer = datos.indexOf("tiempoMacer=");
      String stiempoMacer = "";
      for (int i = ptiempoMacer + 12; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoMacer += datos[i];
      }
      //tiempoMacer = stiempoMacer.toInt();
      numParametros = count(stiempoMacer);
      for (int i = 0;i < numParametros;i ++){
        tiempoMacer[i] = s.separa(stiempoMacer, ':', i);
      }
      

    //Procesar datos de la Temperatura de Cocción
      int ptempCoc = datos.indexOf("tempCoc=");
      String stempCoc = "";
      for (int i = ptempCoc + 8; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempCoc += datos[i];
      }
      //tempCoc = stempCoc.toFloat();
      numParametros = count(stempCoc);
      for (int i = 0;i < numParametros;i ++){
        tempCoc[i] = s.separa(stempCoc, ':', i);
      }
      

    //Procesar datos tiempo de Cocción
      int ptiempoCoc = datos.indexOf("tiempoCoc=");
      String stiempoCoc = "";
      for (int i = ptiempoCoc + 10; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoCoc += datos[i];
      }
      //tiempoCoc = (long) strtol(stiempoCoc.c_str(),NULL,0);
      numParametros = count(stiempoCoc);
      for (int i = 0;i < numParametros;i ++){
        tiempoCoc[i] = s.separa(stiempoCoc, ':', i);
      }
      

    //Procesar datos tiempo del Fermentación
      int ptempFermen = datos.indexOf("tempFermen=");
      String stempFermen = "";
      for (int i = ptempFermen + 11; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempFermen += datos[i];
      }
      //tempFermen = stempFermen.toFloat();
      numParametros = count(stempFermen);
      for (int i = 0;i < numParametros;i ++){
        tempFermen[i] = s.separa(stempFermen, ':', i);
      }
      

    //Procesar datos de la Temperatura de Fermentación
      int ptiempoFermen = datos.indexOf("tiempoFermen=");
      String stiempoFermen = "";
      for (int i = ptiempoFermen + 13; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoFermen += datos[i];
      }
      //tiempoFermen = stiempoFermen.toInt();
      numParametros = count(stiempoFermen);
      for (int i = 0;i < numParametros;i ++){
        tiempoFermen[i] = s.separa(stiempoFermen, ':', i);
      }
      

    //Mostrar información de la receta por Serial
  if (tempMacer != 0){
      //Nombre de la cerveza
        Serial.print("Nombre de la cerveza= ");
        Serial.println(nombre);
      //Temperaturas
        Serial.print("Temperatura del proceso Maceración= ");
        //Serial.println(tempMacer);
        Serial.println(stempMacer);
        Serial.print("Temperatura del proceso Cocción= ");
        Serial.println(stempCoc);
        Serial.print("Temperatura del proceso de Fermentación= ");
        Serial.println(stempFermen);
      //Tiempos en segundos
        Serial.print("Tiempo en Minutos del proceso Maceración= ");
        Serial.println(stiempoMacer);
        Serial.print("Tiempo en Segundos del proceso Cocción= ");
        Serial.println(stiempoCoc);
        Serial.print("Tiempo en Meses del proceso Fermentación= ");
        Serial.println(stiempoFermen);
    }else{
      Serial.println("La receta no existe");
    }
    }else{

      Serial.println("El servidor no responde");

    }

}
}
