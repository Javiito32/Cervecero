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
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje);             // Mandamos la informacion a la BDD a la tabla info
  }

  #ifdef pantallaLCD
    String lcd0 = "Maceracion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif

  digitalWrite(bombaRecirculacion, LOW);
  calentar(Recipe.getTempMacer(faseProceso), Recipe.getTimeMacer(faseProceso));
  digitalWrite(resis, HIGH);
  digitalWrite(bombaRecirculacion, HIGH);
  Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, 2, tiempoRestante, 100);
  
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
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje);
  }

  #ifdef pantallaLCD
    String lcd0 = "Coccion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif


  digitalWrite(bombaRecirculacion, LOW);
  calentar(Recipe.getTempCoc(faseProceso), Recipe.getTimeCoc(faseProceso));
  digitalWrite(resis,HIGH);
  digitalWrite(bombaRecirculacion, HIGH);
  Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, 2, tiempoRestante, 100);
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
  
  Serial.println("O3");

  procesoActual = 4;
  estado = 1;

  if(!recovery) {

    porcentaje = 0;
    Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje);
  }

  #ifdef pantallaLCD
    String lcd0 = "Fermentacion: ";
    lcd0.concat(faseProceso);
    String lcd1 = "Porcentaje: ";
    lcd1.concat(porcentaje);
    lcd1.concat("%");
    printLCD(0, 0, lcd0, 1, 0, lcd1);
  #endif
  
//LECTURA DE VARIABLES
  float temperaturaFermentacion = Recipe.getTempFermen(faseProceso);
  int tiempoFermentacion = Recipe.getTimeFermen(faseProceso);

  if (recovery){

    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(month(recoveryTiempoRestante));
    Serial.print(":");
    Serial.print(day(recoveryTiempoRestante));
    Serial.print(":");
    Serial.println(hour(recoveryTiempoRestante));
    Serial.println("------------------------");

    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoFermentacion * 2629750);
    tiempoProcesoSeg = tiempof - tiempoi;
    tiempof = tiempoi + recoveryTiempoRestante;
  }else {

    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(month(tiempoFermentacion));
    Serial.print(":");
    Serial.print(day(tiempoFermentacion));
    Serial.println(":");
    Serial.print(hour(tiempoFermentacion));
    Serial.println("------------------------");

    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoFermentacion * 2629750);
    tiempoProcesoSeg = tiempof - tiempoi;
  }

  int tiempoLog = tiempoActual + 5;
  int tiempoPorcentaje = tiempoActual + 2;

  do{

    gettime();                                            // Obtiene el tiempo para usarlo en la función
    tiempoRestante = tiempof - tiempoActual;

    if (tiempoActual >= tiempoLog){     
                // Comprueba si han pasado 5 seg, y ejecuta
      tiempoLog = tiempoActual + 5;
      Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje);
    }

    comprobarCancelar();
    if (processCandeled) break;

    if (tiempoRestante <= 0) break;   // Comprueba si el tiempo del proceso es 0, es decir, ha acabado

    if (tiempoActual >= tiempoPorcentaje){              // Comprueba si han pasado 2 seg, y ejecuta

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
    delay(1000);

  } while (true);
  
  Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, 2, tiempoRestante, 100);

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
  
    Serial.println("O4");

    procesoActual = 3;
    estado = 1;
    tiempoRestante = 0;
    if(!recovery){
      porcentaje = 0;
      Log(id_Board, Recipe.getRecipe(), procesoActual, 0, estado, tiempoRestante, porcentaje);
    }
    #ifdef pantallaLCD
      printLCD(0, 0, "Trasvasando... ", 1, 0, "Por favor espere");
    #endif

//Trasvase ON
  digitalWrite(bombaFrio,LOW);
  delay(retrasoBombas);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);

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
      if (tiempoRestante <= 0 || sensorLiquido == HIGH) break;
    delay(500);

  }while(true);
//Trasvase OFF  
  digitalWrite(bombaTrasvase, HIGH);
  digitalWrite(peltier, HIGH);
  digitalWrite(bombaFrio, HIGH);

  Log(id_Board, Recipe.getRecipe(), procesoActual, 0, 2, tiempoRestante, 100);
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
      if (tiempoActual >= tiempoLog){
        tiempoLog = tiempoActual + 5;
        Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, porcentaje);

        
      }
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
    int sensorTemperatura = 25;// analogRead(pinSonda);
    float milivoltios = (sensorTemperatura / 1023.0) * 3300;
    float celsius = milivoltios / 10;
  //Mantenimiento de la ventana de temperatura
    if(celsius > tmax){digitalWrite(resis,HIGH);}
    if(celsius < tmin){digitalWrite(resis,LOW);}
    delay(500);

  }while(true);
}

void comprobarCancelar() {

  if (WiFi.status() == WL_CONNECTED){

    if (!mqttClient.loop()) reconnect();

    if (processCandeled) Serial.println("El proceso se canceló");
    

  }
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
