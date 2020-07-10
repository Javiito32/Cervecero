/*
 * Metodo calentar. 
 * Calienta a la temperatura indicada +- "rangoTemp".
 * Si tenia un proceso pendiente debido a un fallo eléctrico, lo restaurará
 */
void calentar( int temperaturaProceso, long tiempoProceso){

  //Tratamiento de la ventana de temperatura
  
  if (recovery == 1){
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(minute(recoveryTiempoRestante));
    Serial.println(" Minutos");
    Serial.println("------------------------");
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoProceso * 60);
    tiempoProcesoSeg = tiempof - tiempoi;
    tiempof = tiempoi + recoveryTiempoRestante;
  }else{
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(tiempoProceso);
    Serial.println(" Minutos");
    Serial.println("------------------------");
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoProceso * 60);
    tiempoProcesoSeg = tiempof - tiempoi;
  }
    
    long tiempoCancelacion = tiempoActual + 5;
    int tiempoPorcentaje = tiempoActual + 2;

    int tmax = temperaturaProceso+rangoTemp;
    int tmin = temperaturaProceso-rangoTemp;
    do{
      if (tiempoActual >= tiempoCancelacion){
        tiempoCancelacion = tiempoActual + 5;
        comprobarCancelar();
        sendInfo(procesoActual,pasoProceso);
        if (falloProceso){
          break;
        }
      }
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      if (tiempoRestante <= 0) {porcentaje = 100; lcd_Porcentaje(); break; }
      if (tiempoActual >= tiempoPorcentaje){
        tiempoPorcentaje = tiempoActual + 2;
        int timepoIncremental = tiempoProcesoSeg - tiempoRestante;
        porcentaje = (timepoIncremental * 100) / tiempoProcesoSeg;
        Serial.print("Leeva el ");
        Serial.print(porcentaje);
        Serial.print("%");
        Serial.println(" completado");
        lcd_Porcentaje();
      }
      
  //Tratamiento de la temperatura
    int sensorTemperatura = analogRead(pinSonda);
    float milivoltios = (sensorTemperatura / 1023.0) * 3300;
    float celsius = milivoltios / 10;
  //Mantenimiento de la ventana de temperatura
    if(celsius > tmax){digitalWrite(resis,LOW);}
    if(celsius < tmin){digitalWrite(resis,HIGH);}
    delay(1000);
  }while(true);
  
//PUESTA A CERO FINAL
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
}


/*
 * Metodo para recircular. 
 * Sirve poner los reles y bombas en la posicion de recirculacion.
 * 
 * No tiene parametros
 * No devuelve nada
 */
void recircular(){
  digitalWrite(bombaRecirculacion,HIGH);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
}

void comprobarCancelar() {
  if (WiFi.status() == WL_CONNECTED){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    String datos = peticion("checkCancel.php",datos_Enviar);
      if (datos != "fallo") {
        int cancelar = datos.toInt();
        if (cancelar == 1){
          falloProceso = 1;
        }
    }else{
      Serial.println("No se pudo comprobar la cancelación del proceso");
    }
   }
}

/*
 * Metodo para enviar el final del proceso con errores.
 * Envia un mensaje con la informacion final del proceso a la BDD
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
      String datos_Enviar = "IDplaca=";
      datos_Enviar.concat(IDplaca);
      peticion("resetCancelar.php",datos_Enviar);
      falloProceso = 0;
    }
  }
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}

void lcd_Porcentaje(){
  lcd.setCursor(12,1);                                      // Ponemos el cursor para empezar a escrivir en la linea 2 celda 12
  String lcd2 = ""; 
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
}
