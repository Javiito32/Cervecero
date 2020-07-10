void SQL_menu(){
  while(true){
  delay(100);
  if (WiFi.status() == WL_CONNECTED){
    String datos_Enviar = "menu=";
    datos_Enviar.concat("1&IDplaca=");
    datos_Enviar.concat(IDplaca);
    peticion("menu.php",datos_Enviar);    
    }
  }
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
  SQL_menu();
  IDreceta = dato;
  leerReceta();
}

void procesos(){
  if (IDreceta == 0){
    Serial.println("Primero selecciona una receta");
    return;
  }
  Serial.println("Selecciona proceso: ");
  SQL_menu();
       if (dato==1) { SQL_menu(); pasoProceso = dato; maceracion(); }
  else if (dato==2) { SQL_menu(); pasoProceso = dato; coccion();}
  else if (dato==3) { trasvase();}
  else if (dato==4) { SQL_menu(); pasoProceso = dato; fermentacion();}
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
  SQL_menu();
       if (dato==1) { time_set();}
  else if (dato==2) { showtime();}
  else if (dato==3) { tonos();}
  
  else Serial.println("La accion deseada no existe");
}



void tonos(){
  Serial.println("------------------------------");
  Serial.println("Selecciona Música: ");
  Serial.println("------------------------------");
  SQL_menu();
      if (dato==1) { c_nokia_c();}
  else Serial.println("La canción no existe");
}
