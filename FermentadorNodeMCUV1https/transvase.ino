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
    }else{
      procesoActual = 3;
      estado = 1;
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
        sendInfo(procesoActual,0);
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
