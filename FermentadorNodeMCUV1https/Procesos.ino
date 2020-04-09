/*  
 *  Funcion para realizar MACERACION.
 *  Primero se comprueba si el proceso se tiene que recuperar o no.
 *  Después, se cogen las variables de la temperatura y el tiempo del proceso.
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento manteniendo la tempetatura durante el tiempo del proceso 
 *  Cada cierto tiempo se envia información a la base de datos para el seguimiento del proceso.
 *  Además se comprueba si se ha cancelado el proceso por parte del usuario
 *  
 */

  
void maceracion (){

  if(recovery == 1){
    procesoActual = 1;
    estado = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    String lcd1 = "Maceracion: ";
    lcd1.concat(pasoProceso);
    lcd.print(lcd1);
    lcd.setCursor(0,1);
    String lcd2 = "Porcentaje: ";
    lcd2.concat(porcentaje);
    lcd2.concat("%");
    lcd.print(lcd2);
  
  }else{
//Confirmacion del inicio de proceso de maceracion
  Serial.println("O1");
  
//Configuracion del proceso
  procesoActual = 1;
  estado = 1;
  porcentaje = 0;
  sendInfo(procesoActual,pasoProceso);
  lcd.clear();
  lcd.setCursor(0,0);
  String lcd1 = "Maceracion: ";
  lcd1.concat(pasoProceso);
  lcd.print(lcd1);
  lcd.setCursor(0,1);
  String lcd2 = "Porcentaje: ";
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
}
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[pasoProceso].toFloat();           //Variable con la temperatura del proceso
  int tiempoMaceracion = tiempoMacer[pasoProceso].toInt();                  //Variable del tiempo del proceso en minutos
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
  
//Envio mensaje de fin de proceso.
  if (falloProceso) {estado = 3; porcentaje = 100;}
  else {estado = 2; c_nokia_c(); porcentaje = 100;};
  recovery = 0;
  sendInfo(procesoActual,pasoProceso);
  finProceso(procesoActual,falloProceso);
  
}



/*
 *  Funcion para realizar COCCION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  La consigna empieza por "T", seguida de cuatro numeros (centenas de temperatura, decenas de 
 *  temperatura, unidades de temperatura y decimas de temperatura), seguidas por "S" y el tiempo
 *  del proceso en segundos, acabando la consigna en "."
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la constante.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void coccion (){ 
  if(recovery == 1){
    procesoActual = 2;
    estado = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    String lcd1 = "Coccion: ";
    lcd1.concat(pasoProceso);
    lcd.print(lcd1);
    lcd.setCursor(0,1);
    String lcd2 = "Porcentaje: ";
    lcd2.concat(porcentaje);
    lcd2.concat("%");
    lcd.print(lcd2);
  
  }else{
//Confirmacion del inicio de proceso de maceracion
  Serial.println("O2");
//Configuracion del proceso
  procesoActual = 2;
  estado = 1;
  porcentaje = 0;
  sendInfo(procesoActual,pasoProceso);
  lcd.clear();
  lcd.setCursor(0,0);
  String lcd1 = "Coccion: ";
  lcd1.concat(pasoProceso);
  lcd.print(lcd1);
  lcd.setCursor(0,1);
  String lcd2 = "Porcentaje: ";
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
} 
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[pasoProceso].toFloat();           //Variable con la temperatura del proceso
  int tiempoMaceracion = tiempoMacer[pasoProceso].toInt();                  //Variable del tiempo del proceso en minutos
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
//Envio mensaje de fin de proceso.
  if (falloProceso) {estado = 3; porcentaje = 100;}
  else {estado = 2; c_nokia_c(); porcentaje = 100;};
  lcd_Porcentaje();
  recovery = 0;
  sendInfo(procesoActual,pasoProceso);
  finProceso(procesoActual,falloProceso);
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
  if (recovery != 1){
//Confirmacion del inicio de proceso de trasvase
    Serial.println("O3");
    procesoActual = 3;
    estado = 1;
    tiempoRestante = 0;
    porcentaje = 0;
    sendInfo(procesoActual,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Trasvasando... ");
    lcd.setCursor(0,1);
    lcd.print("Por favor espere");
    }else{
      procesoActual = 3;
      estado = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Trasvasando... ");
      lcd.setCursor(0,1);
      lcd.print("Por favor espere");
    }
   
//Trasvase ON
  digitalWrite(bombaFrio,HIGH);
  delay(retrasoBombas);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(peltier,HIGH);

//Control de tiempo y sensor de liquido
    Serial.println("------------------------");
    Serial.print("El tiempo de seguridad es de: ");
    Serial.print("4");
    Serial.println(" Minutos");
    Serial.println("------------------------");
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
  
//Envio mensaje de fin de proceso.
  if (falloProceso) {estado = 3; porcentaje = 100;}
  else {estado = 2; c_nokia_c(); porcentaje = 100;};
  recovery = 0;
  sendInfo(procesoActual,0);
  finProceso(procesoActual,falloProceso);
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
  if (recovery == 1){
    procesoActual = 4;
    estado = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    String lcd1 = "Fermentacion: ";
    lcd1.concat(pasoProceso);
    lcd.print(lcd1);
    lcd.setCursor(0,1);
    String lcd2 = "Porcentaje: ";
    lcd2.concat(porcentaje);
    lcd2.concat("%");
    lcd.print(lcd2);
  }else{
  Serial.println("O4");
  procesoActual = 4;
  estado = 1;
  porcentaje = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  String lcd1 = "Fermentacion: ";
  lcd1.concat(pasoProceso);
  lcd.print(lcd1);
  lcd.setCursor(0,1);
  String lcd2 = "Porcentaje: ";
  lcd2.concat(porcentaje);
  lcd2.concat("%");
  lcd.print(lcd2);
  sendInfo(procesoActual,pasoProceso);
  }
  
//LECTURA DE VARIABLES
  float temperaturaFermentacion = tempFermen[pasoProceso].toFloat();
  int tiempoFermentacion = tiempoFermen[pasoProceso].toInt();
  if (recovery == 1){
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
  }else{
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

  long tiempoCancelacion = tiempoActual + 5;
  int tiempoPorcentaje = tiempoActual + 2;
  do{
    gettime();                                            // Obtiene el tiempo para usarlo en la función
    tiempoRestante = tiempof - tiempoActual;
    if (tiempoActual >= tiempoCancelacion){
      tiempoCancelacion = tiempoActual + 5;
      comprobarCancelar();
      sendInfo(procesoActual,pasoProceso);
      if (falloProceso){
        break;
      }
    }
    if (tiempoRestante <= 0) {break; porcentaje = 100;}
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
      delay(1000);
  }while(true);
  
//Envio mensaje de fin de proceso.
  if (falloProceso) {estado = 3; porcentaje = 100;}
  else {estado = 2; c_nokia_c(); porcentaje = 100;};
  lcd_Porcentaje();
  recovery = 0;
  sendInfo(procesoActual,pasoProceso);
  finProceso(procesoActual,falloProceso);
}
