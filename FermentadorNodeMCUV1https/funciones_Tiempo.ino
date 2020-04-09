void showtime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
  Serial.println(tiempoActual);
  DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
            // DateTime y asigna a variable fecha
        Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
        Serial.print(" ");       // caracter espacio en blanco como separador
        Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
}

void gettime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
}

void time_set (){
  if (WiFi.status() == WL_CONNECTED){
    String stringtime = peticion("time.php","");
      if (stringtime != "fallo") {
        unsigned long timeset = (long) strtol(stringtime.c_str(),NULL,0);
        rtc.adjust(DateTime(year(timeset),month(timeset),day(timeset),hour(timeset),minute(timeset),second(timeset)));
        DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
            // DateTime y asigna a variable fecha
        Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
        Serial.print(" ");       // caracter espacio en blanco como separador
        Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
      }
  }
 }
