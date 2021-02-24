/*  
 *  Funcion para realizar MACERACION.
 *  Primero se comprueba si el proceso se quedase a mitad en caso de corte eléctrico, en caso verdadero, reanudará el proceso.
 *  Después, se recuperan las variables de la temperatura y el tiempo del proceso.
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento manteniendo la tempetatura durante el tiempo del proceso 
 *  Cada cierto tiempo se envia información a la base de datos para el seguimiento del proceso.
 *  Además se comprueba si se ha cancelado el proceso por parte del usuario
 *  
 */

byte porcentaje;
unsigned long tiempoi;                                // Tiempo inicial para los procesos en seg
unsigned long tiempof;                                // Tiempo final para los procesos en seg
unsigned long tiempoActual;                           // Tiempo actual del proceso en seg
long tiempoRestante;                                  // Tiempo que falta para el final de los procesos en seg
unsigned long tiempoTrans;                            // Tiempo transvase de la recta selecionada
bool processCandeled = false;                         // Guarda si falla el tiempo
byte procesoActual;                                   // El proceso que se esta ejecutando
byte estado;                                          // 1 - Iniciado, 2 - Finalizado, 3 - Cancelado
long tiempoProcesoSeg;                                // El tiempo del proceso en segundos

  
void maceracion() {
  
  if(checkLoadRecipe()){return;};
  
  Serial.println("O1");

  procesoActual = 1;
  estado = 1;

  if(!recovery){
    porcentaje = 0;
    tiempoRestante = Recipe.getTimeMacer(faseProceso);
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje, 0.0);             // Mandamos la informacion a la BDD a la tabla info
  }

  #ifdef pantallaLCD
    String lcd0 = "Maceracion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif

  expander.digitalWrite(electroRecirculacion, LOW);
  expander.digitalWrite(bombaPrincipal, LOW);
  calentar(Recipe.getTempMacer(faseProceso), Recipe.getTimeMacer(faseProceso));
  digitalWrite(HEATER, LOW);
  expander.digitalWrite(bombaPrincipal, HIGH);
  expander.digitalWrite(electroRecirculacion, HIGH);

  endProcess();
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
  
  Serial.println("O2");

  procesoActual = 2;
  estado = 1;

  if(!recovery){
    porcentaje = 0;
    tiempoRestante = Recipe.getTimeCoc(faseProceso);
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje, 0.0);
  }

  #ifdef pantallaLCD
    String lcd0 = "Coccion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif

  expander.digitalWrite(electroRecirculacion, LOW);
  expander.digitalWrite(bombaPrincipal, LOW);
  calentar(Recipe.getTempCoc(faseProceso), Recipe.getTimeCoc(faseProceso));
  digitalWrite(HEATER, LOW);
  expander.digitalWrite(bombaPrincipal, HIGH);
  expander.digitalWrite(electroRecirculacion, HIGH);

  endProcess();
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
  
    Serial.println("O3");

    procesoActual = 3;
    estado = 1;
    tiempoRestante = 0;
    if(!recovery){
      porcentaje = 0;
      Log(id_Board, Recipe.getRecipe(), procesoActual, 0, estado, tiempoRestante, porcentaje, 0.0);
    }
    #ifdef pantallaLCD
      printLCD(0, 0, "Trasvasando... ", 1, 0, "Por favor espere");
    #endif

//Trasvase ON
  digitalWrite(LEDTRASVASE, HIGH);
  expander.digitalWrite(electroTrasvase, LOW);
  delay(1000);
  expander.digitalWrite(bombaPrincipal, LOW);
  expander.digitalWrite(bombaFrio, LOW);
  


//Control de tiempo y sensor de liquido
    Serial.println("------------------------");
    Serial.print("El tiempo de seguridad es de: ");
    Serial.print("4");
    Serial.println(" Minutos");
    Serial.println("------------------------");

    gettime();                            
    tiempoi = tiempoActual;
    tiempof = tiempoi + (4 * 60);

    do{
      
    gettime();
    tiempoRestante = tiempof - tiempoActual;
    comprobarCancelar();
    if (processCandeled) break;
    if (tiempoRestante <= 0 || digitalRead(TUBESENSOR) == LOW) break;
    delay(500);

  }while(true);
//Trasvase OFF  
  digitalWrite(LEDTRASVASE, LOW);
  expander.digitalWrite(bombaPrincipal, HIGH);
  expander.digitalWrite(bombaFrio, HIGH);
  expander.digitalWrite(electroTrasvase, HIGH);

  estado = 2;

  endProcess();
}

/* Comprueba si hay una receta cargada*/
bool checkLoadRecipe() {

  if(Recipe.getRecipe() == 0){ 

    #ifdef pantallaLCD
      printLCD(0, 0, "No hay receta", 1, 0, "");
    #endif
    delay(2000);
    return true;
  }else return false;
  
}

void fermentacion() {
  
  if(checkLoadRecipe()){return;};
  
  Serial.println("O4");

  procesoActual = 4;
  estado = 1;

  if(!recovery){
    porcentaje = 0;
    tiempoRestante = Recipe.getTimeMacer(faseProceso);
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje, 0.0);             // Mandamos la informacion a la BDD a la tabla info
  }

  #ifdef pantallaLCD
    String lcd0 = "Fermentacion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif

  expander.digitalWrite(electroRecirculacion, LOW);
  expander.digitalWrite(bombaPrincipal, LOW);
  calentar(Recipe.getTempFermen(faseProceso), Recipe.getTimeFermen(faseProceso));
  digitalWrite(HEATER, LOW);
  expander.digitalWrite(bombaPrincipal, HIGH);
  expander.digitalWrite(electroRecirculacion, HIGH);
  
  endProcess();
}


/*
 * Metodo calentar. 
 * Calienta a la temperatura indicada +- "rangoTemp".
 * Si tenia un proceso pendiente debido a un fallo eléctrico, lo restaurará
 */
void calentar(int temperaturaProceso, long tiempoProceso){
  
  if (recovery){

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
    
    int tiempoLog = tiempoActual + 5;
    int tiempoPorcentaje = tiempoActual + 2;

    int tmax = temperaturaProceso + rangoTemp;
    int tmin = temperaturaProceso - rangoTemp;

    do{
      comprobarCancelar();
      if (processCandeled) break;
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

        Serial.print("Leeva el ");
        Serial.print(porcentaje);
        Serial.print("%");
        Serial.println(" completado");

        #ifdef pantallaLCD
          lcd_Porcentaje();
        #endif
      }
      
      //Tratamiento de la temperatura
      for (size_t i = 0; i < 10; i++) {

        samples.add(((analogRead(SONDA) * 5000) / 1023) / 10);
      }
    
      float temp = samples.getAverage();
      //Serial.println(temp);
      samples.clear();
      // Mandar LOG
      if (tiempoActual >= tiempoLog){

        tiempoLog = tiempoActual + 5;
        Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje, temp);
      }
    //Mantenimiento de la ventana de temperatura
      if(temp > tmax) {digitalWrite(HEATER,LOW);}
      if(temp < tmin) {digitalWrite(HEATER,HIGH);}
      delay(500);

    }while(true);
}

void comprobarCancelar() {

  if (WiFi.status() == WL_CONNECTED){

    if (!mqttClient.loop()) reconnect();

    if (processCandeled) Serial.println("El proceso se canceló");
    

  }
}

void endProcess() {

  if (processCandeled) {estado = 3;}
  else {estado = 2;}
  
  Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, 100, 0.0);
}

#ifdef pantallaLCD
void lcd_Porcentaje(){

  lcd.setCursor(12,1);
  String lcd1 = ""; 
  lcd1.concat(porcentaje);
  lcd1.concat("%");
  lcd.print(lcd1);
}
#endif
