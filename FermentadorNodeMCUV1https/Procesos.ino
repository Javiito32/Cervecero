/*  
 *  Funcion para realizar MACERACION.
 *  Primero se comprueba si el proceso se quedase a mitad en caso de corte eléctrico, en caso verdadero, reanudará el proceso.
 *  Después, se recuperan las variables de la temperatura y el tiempo del proceso.
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento manteniendo la tempetatura durante el tiempo del proceso 
 *  Cada cierto tiempo se envia información a la base de datos para el seguimiento del proceso.
 *  Además se comprueba si se ha cancelado el proceso por parte del usuario
 *  
 */

  
void maceracion (){
  if(checkLoadRecipe()){return;};
  
  #ifdef debug
    Serial.println("O1");
  #endif
  procesoActual = 1;
  estado = 1;
  if(!recovery){
    porcentaje = 0;
    Log(procesoActual,faseProceso);             // Mandamos la informacion a la BDD a la tabla info
  }
  #ifdef pantallaLCD
  lcd.clear();                                // Limpia lo que hubiese escrito en la lcd
  lcd.setCursor(0,0);                         // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
  String lcd1 = "Maceracion: ";
  lcd1.concat(faseProceso);
  lcd.print(lcd1);
  lcd.setCursor(0,1);                         // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
  String lcd2 = "Porcentaje: ";
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
  #endif
//}
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[faseProceso].toFloat();           // Pasa la variable de temperatura de una String a un Float
  int tiempoMaceracion = tiempoMacer[faseProceso].toInt();                  // Pasa la variable de tiempo de una String a un Int
//MODO RECIRCULACION
  recircular();
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
  finishProcess();
  
}



/*
 *  Funcion para realizar COCCION.
 *  Primero se comprueba si el proceso se quedase a mitad en caso de corte eléctrico, en caso verdadero, reanudará el proceso.
 *  Después, se recuperan las variables de la temperatura y el tiempo del proceso.
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la variable rangoTemp.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void coccion (){ 
  
  if(checkLoadRecipe()){return;}; 
  
  #ifdef debug
    Serial.println("O2");
  #endif
  procesoActual = 2;
  estado = 1;
  if(!recovery){
    porcentaje = 0;
    Log(procesoActual,faseProceso);
  }
  #ifdef pantallaLCD
    lcd.clear();                                                          // Limpia lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    String lcd1 = "Coccion: ";
    lcd1.concat(faseProceso);
    lcd.print(lcd1);  
    lcd.setCursor(0,1);                                                   // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
    String lcd2 = "Porcentaje: ";
    lcd2.concat(porcentaje);
    lcd2.concat("%");
    lcd.print(lcd2);
  #endif
//} 
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[faseProceso].toFloat();           // Pasa la variable de temperatura de una String a un Float
  int tiempoMaceracion = tiempoMacer[faseProceso].toInt();                  // Pasa la variable de tiempo de una String a un Int
//MODO RECIRCULACION
  recircular();
  
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
  finishProcess();
}


/*
 *  Funcion para realizar FERMENTACION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  Finaliza el proceso cuando acaba el tiempo
 *  Encripta las variables y las envia.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void fermentacion(){
  
  if(checkLoadRecipe()){return;};
  
  #ifdef debug
    Serial.println("O3");
  #endif
  procesoActual = 4;
  estado = 1;
  if(!recovery){
    porcentaje = 0;
    Log(procesoActual,faseProceso);
  }
  #ifdef pantallaLCD
    lcd.clear();                                                                    // Limpia lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                                             // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    String lcd1 = "Fermentacion: ";
    lcd1.concat(faseProceso);
    lcd.print(lcd1);
    lcd.setCursor(0,1);                                                             // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
    String lcd2 = "Porcentaje: ";
    lcd2.concat(porcentaje);
    lcd2.concat("%");
    lcd.print(lcd2);
  #endif
  //}
  
//LECTURA DE VARIABLES
  float temperaturaFermentacion = tempFermen[faseProceso].toFloat();              // Pasa la variable de temperatura de una String a un Float
  int tiempoFermentacion = tiempoFermen[faseProceso].toInt();                     // Pasa la variable de tiempo de una String a un Int
  if (recovery == 1){
    #ifdef debug
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(month(recoveryTiempoRestante));
    Serial.print(":");
    Serial.print(day(recoveryTiempoRestante));
    Serial.print(":");
    Serial.println(hour(recoveryTiempoRestante));
    Serial.println("------------------------");
    #endif
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoFermentacion * 2629750);
    tiempoProcesoSeg = tiempof - tiempoi;
    tiempof = tiempoi + recoveryTiempoRestante;
  }else{
    #ifdef debug
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(month(tiempoFermentacion));
    Serial.print(":");
    Serial.print(day(tiempoFermentacion));
    Serial.println(":");
    Serial.print(hour(tiempoFermentacion));
    Serial.println("------------------------");
    #endif
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoFermentacion * 2629750);
    tiempoProcesoSeg = tiempof - tiempoi;
  }

  long tiempoCancelacion = tiempoActual + 5;
  int tiempoPorcentaje = tiempoActual + 2;
  do{
    gettime();                                            // Obtiene el tiempo para usarlo en la función
    tiempoRestante = tiempof - tiempoActual;
    if (tiempoActual >= tiempoCancelacion){               // Comprueba si han pasado 5 seg, y ejecuta
      tiempoCancelacion = tiempoActual + 5;
      comprobarCancelar();
      Log(procesoActual,faseProceso);
      if (falloProceso){
        break;
      }
    }
    if (tiempoRestante <= 0) {break;}   // Comprueba si el tiempo del proceso es 0, es decir, ha acabado
      if (tiempoActual >= tiempoPorcentaje){              // Comprueba si han pasado 2 seg, y ejecuta
        tiempoPorcentaje = tiempoActual + 2;
        int timepoIncremental = tiempoProcesoSeg - tiempoRestante;
        porcentaje = (timepoIncremental * 100) / tiempoProcesoSeg;
        #ifdef debug
        Serial.print("Leeva el ");
        Serial.print(porcentaje);
        Serial.print("%");
        Serial.println(" completado");
        #endif
        #ifdef pantallaLCD
        lcd_Porcentaje();
        #endif
      }
      delay(1000);
  }while(true);
  
  finishProcess();

}


/*
 *  Funcion para realizar TRASVASE.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso y activa las 
 *  bombas y reles necesarios.
 *  Se pone en modo trasvase hasta que recibe un mensaje de fin que viene dado por la 
 *  siguiente consigna: "T0000S0."
 *    
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void trasvase(){
  
    #ifdef debug
      Serial.println("O4");
    #endif
    procesoActual = 3;
    estado = 1;
    tiempoRestante = 0;
    if(!recovery){
      porcentaje = 0;
      Log(procesoActual,0);
    }
    #ifdef pantallaLCD
      lcd.clear();                                                      // Limpia lo que hubiese escrito en la lcd
      lcd.setCursor(0,0);                                               // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
      lcd.print("Trasvasando... ");
      lcd.setCursor(0,1);                                               // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
      lcd.print("Por favor espere");
    #endif
   
//Trasvase ON
  digitalWrite(bombaFrio,HIGH);
  delay(retrasoBombas);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(peltier,HIGH);

//Control de tiempo y sensor de liquido
    #ifdef debug
      Serial.println("------------------------");
      Serial.print("El tiempo de seguridad es de: ");
      Serial.print("4");
      Serial.println(" Minutos");
      Serial.println("------------------------");
    #endif
    gettime();                            
    tiempoi = tiempoActual;
    tiempof = tiempoi + (4 * 60);
    long tiempoCancelacion = tiempoActual + 5;
    do{
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      if (tiempoActual >= tiempoCancelacion){
        tiempoCancelacion = tiempoActual + 5;
        comprobarCancelar();
        if (falloProceso){
          break;
        }
      }
      if (tiempoRestante <= 0 || sensorLiquido == HIGH) break;
    delay(1000);
  }while(true);
//Trasvase OFF  
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
  finishProcess();
}

bool checkLoadRecipe() {
  if(IDreceta == 0){                             // Comprueba si hay una receta cargada
    #ifdef pantallaLCD
      lcd.clear();                               // Limpia lo que hubiese escrito en la lcd     
      lcd.setCursor(0,0);                        // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
      lcd.print("No hay receta");
    #endif
    delay(2000);
    return true;
  } else {
    return false;
  }
}

void finishProcess() {
  //Envio mensaje de fin de proceso.
  if (falloProceso) {estado = 3; porcentaje = 100;}
  else {estado = 2; c_nokia_c(); porcentaje = 100;};
  #ifdef pantallaLCD
    lcd_Porcentaje();
  #endif
  recovery = 0;
  Log(procesoActual,faseProceso);
  finProceso(procesoActual,falloProceso);
}

/*
 * Metodo calentar. 
 * Calienta a la temperatura indicada +- "rangoTemp".
 * Si tenia un proceso pendiente debido a un fallo eléctrico, lo restaurará
 */
void calentar( int temperaturaProceso, long tiempoProceso){

  //Tratamiento de la ventana de temperatura
  
  if (recovery){
    #ifdef debug
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(minute(recoveryTiempoRestante));
    Serial.println(" Minutos");
    Serial.println("------------------------");
    #endif
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoProceso * 60);
    tiempoProcesoSeg = tiempof - tiempoi;
    tiempof = tiempoi + recoveryTiempoRestante;
  }else{
    #ifdef debug
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(tiempoProceso);
    Serial.println(" Minutos");
    Serial.println("------------------------");
    #endif
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
        Log(procesoActual,faseProceso);
        if (falloProceso){
          break;
        }
      }
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      if (tiempoRestante <= 0) {
        porcentaje = 100;
        #ifdef pantallaLCD
          lcd_Porcentaje();
        #endif
        break; 
      }
      if (tiempoActual >= tiempoPorcentaje){
        tiempoPorcentaje = tiempoActual + 2;
        int timepoIncremental = tiempoProcesoSeg - tiempoRestante;
        porcentaje = (timepoIncremental * 100) / tiempoProcesoSeg;
        #ifdef debug
          Serial.print("Leeva el ");
          Serial.print(porcentaje);
          Serial.print("%");
          Serial.println(" completado");
        #endif
        #ifdef pantallaLCD
          lcd_Porcentaje();
        #endif
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
      #ifdef debug
      Serial.println("No se pudo comprobar la cancelación del proceso");
      #endif
    }
   }
}

/*
 * Metodo para enviar el final del proceso con errores.
 * Envia un mensaje con la informacion final del proceso a la BDD
 */
void finProceso (int proceso,bool error){

  #ifdef debug
  String mensaje = "Proceso ";
  mensaje.concat(proceso);
  mensaje.concat(" Fallo ");
  mensaje.concat(error);
  Serial.println(mensaje);
  #endif
  
  if (falloProceso){
    if (WiFi.status() == WL_CONNECTED) {
      String datos_Enviar = "IDplaca=";
      datos_Enviar.concat(IDplaca);
      peticion("resetCancelar.php",datos_Enviar);
      falloProceso = 0;
    }
  }
}
#ifdef pantallaLCD
void lcd_Porcentaje(){
  lcd.setCursor(12,1);                                      // Ponemos el cursor para empezar a escrivir en la linea 2 celda 12
  String lcd2 = ""; 
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
}
#endif