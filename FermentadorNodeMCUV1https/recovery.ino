/*
 * Revisión de si hay algún procesos que se ha quedado a medias
 */
void checkrecovery(){
  Serial.println("Comprobando Recovery");
  while (true){
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    String datos = peticion("recovery.php",datos_Enviar);
      if (datos != "fallo") {
        //Serial.println(datos);
        
        //Decode
        int longitud = datos.length();
        int pestado = datos.indexOf("estado=");
        String sestado = "";
        for (int i = pestado + 7; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else sestado += datos[i];
        }
        int estado = sestado.toInt();

        if(estado == 1){
          recovery = 1;
          Serial.println("------------------------------");
          Serial.println("Hay procesos pendiantes");
          Serial.println("------------------------------");


          int preceta = datos.indexOf("receta=");
          String sreceta = "";
          for (int i = preceta + 7; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sreceta += datos[i];
          }
          IDreceta = sreceta.toInt();
          

          int ptiempoRestante = datos.indexOf("tiempoRestante=");
          String stiempoRestante = "";
          for (int i = ptiempoRestante + 15; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else stiempoRestante += datos[i];
          }
          recoveryTiempoRestante = stiempoRestante.toInt();



          int pproceso = datos.indexOf("proceso=");
          String sproceso = "";
          for (int i = pproceso + 8; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sproceso += datos[i];
          }
          recoveryProceso = sproceso.toInt();



          int ppasoProceso = datos.indexOf("pasoProceso=");
          String spasoProceso = "";
          for (int i = ppasoProceso + 12; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else spasoProceso += datos[i];
          }
          recoveryPasoProceso = spasoProceso.toInt();

        Serial.print("Receta a recuperar: ");
        Serial.println(IDreceta);
        Serial.print("Tiempo que le falta: ");
        Serial.println(recoveryTiempoRestante);
        Serial.print("Proceso que estaba: ");
        Serial.println(recoveryProceso);
        Serial.print("Paso del proceso que estaba: ");
        Serial.println(recoveryPasoProceso);
        break;
     
        }else{break;}
      }else{
        Serial.println("------------------------------");
        Serial.println("Error de conexión con el servidor");
        Serial.println("------------------------------");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el Recovery");
      }
  }
}
/*
 * Lanza el proceso por el que iba en caso de no concluir
 */
void recoveryProcesos(int proceso){
       if (proceso==1) { maceracion(); }
  else if (proceso==2) { coccion();}
  else if (proceso==3) { trasvase();}
  else if (proceso==4) { fermentacion();}
  else Serial.println("Proceso no existente");
}
