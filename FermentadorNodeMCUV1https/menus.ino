void recoveryProcesos(int proceso){
       if (proceso==1) { maceracion(); }
  else if (proceso==2) { coccion();}
  else if (proceso==3) { trasvase();}
  else if (proceso==4) { fermentacion();}
  else Serial.println("Proceso no existente");
}

void menuinicio(int n){ 
       if (n==1) { receta();}
  else if (n==2) { procesos();}
  else if (n==3) { ajustes();}
  else if (n==4) { limpieza();}
  else Serial.println("La accion deseada no existe");
}

void receta(){
  Serial.println("------------------------------");
  Serial.println("Selecciona receta: ");
  Serial.println("------------------------------");
  pregunta();
  IDreceta = dato;
  leerReceta();
}

void procesos(){
  if (IDreceta == 0){
    Serial.println("Primero selecciona una receta");
    return;
  }
  Serial.println("Selecciona proceso: ");
  pregunta();
       if (dato==1) { pregunta(); pasoProceso = dato; maceracion(); }
  else if (dato==2) { pregunta(); pasoProceso = dato; coccion();}
  else if (dato==3) { trasvase();}
  else if (dato==4) { pregunta(); pasoProceso = dato; fermentacion();}
  else Serial.println("Proceso no existente");
}


void limpieza(){
    Serial.println("------------------------");
    Serial.println("Se ha iniciado la limpieza");
    Serial.println("    Por favor, espere");
    Serial.println("------------------------");
    trasvase();
}

void ajustes(){
  Serial.println("Ajustes");
  pregunta();
       if (dato==1) { time_set();}
  else if (dato==2) { showtime();}
  else if (dato==3) { tonos();}
  
  else Serial.println("La accion deseada no existe");
}



void tonos(){
  Serial.println("------------------------------");
  Serial.println("Selecciona Música: ");
  Serial.println("------------------------------");
  pregunta();
      if (dato==1) { c_nokia_c();}
  else Serial.println("La canción no existe");
}
