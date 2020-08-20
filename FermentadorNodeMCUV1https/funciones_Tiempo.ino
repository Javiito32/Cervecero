void printTime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
  DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
            // DateTime y asigna a variable fecha
        Serial.println(tiempoActual);
        Serial.print(fecha.day());        // Dia
        Serial.print("/");
        Serial.print(fecha.month());      // Meses
        Serial.print("/");
        Serial.print(fecha.year());       // Años
        Serial.print(" ");
        Serial.print(fecha.hour());       // Horas
        Serial.print(":");
        Serial.print(fecha.minute());     // Minutos
        Serial.print(":");
        Serial.println(fecha.second());   // Segundos 
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
        Serial.print(fecha.day());        // Dia
        Serial.print("/");
        Serial.print(fecha.month());      // Meses
        Serial.print("/");
        Serial.print(fecha.year());       // Años
        Serial.print(" ");
        Serial.print(fecha.hour());       // Horas
        Serial.print(":");
        Serial.print(fecha.minute());     // Minutos
        Serial.print(":");
        Serial.println(fecha.second());   // Segundos 
      }
  }
 }
