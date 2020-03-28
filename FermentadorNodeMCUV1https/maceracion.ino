/*  
 *  Funcion para realizar MACERACION.
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

  
void maceracion (){

  if(recovery == 1){
    procesoActual = 1;
    estado = 1;
  
  }else{
//Confirmacion del inicio de proceso de maceracion
  Serial.println("O1");
//Configuracion del proceso
  procesoActual = 1;
  estado = 1;
  porcentaje = 0;
  sendInfo(procesoActual,pasoProceso);
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
