/* Devuelve fecha y hora en formato */
void printTime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
  DateTime fecha = rtc.now();

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


/* Asigna el valor del tiempo a la variable tiempoActual */
void gettime(){

  DateTime now = rtc.now();
  tiempoActual = now.unixtime();

}

/* Pone el hora el modulo RTC con la hora del servidor */
void time_set (){

  if (WiFi.status() == WL_CONNECTED){

    String stringtime = peticion("time.php","");

      if (stringtime != "fallo") {

        unsigned long timeset = (long) strtol(stringtime.c_str(),NULL,0);
        rtc.adjust(DateTime(year(timeset),month(timeset),day(timeset),hour(timeset),minute(timeset),second(timeset)));
        DateTime fecha = rtc.now();

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
