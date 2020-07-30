/*
 * SQL menu: Este menu, conecta con la BBDD directamente con un php llamado menu.php.
  */
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

/* 
 * El json_menu lo que hace es comprobar si esta conectado a internet y luego recoge datos en formato json,
 * los datos son 3 variables, después resetea los valores que ha cogido de la BDD, por último, ejecuta 
 * la función menu pruebas con los datos recogidos por el json.
 */

// 01
void json_menu(){
  int menu;
  int dato1;
  int dato2;
  while(true){
  delay(100);
  if (WiFi.status() == WL_CONNECTED){
    lcd.setCursor(0,1);
    lcd.print(" Ready   Online");
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    String datosString = peticion("json.php",datos_Enviar);
// 02
      if (datosString != "fallo") {
        const char * datos = datosString.c_str();
        
        const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
        DynamicJsonDocument doc(capacity);

        //const char* json = "{\"menu\":\"1\",\"dato1\":\"1\",\"dato2\":\"1\"}";        //Para testear la decodificación de las variables
        
        deserializeJson(doc, datos);

        menu = doc["menu"];
        dato1 = doc["dato1"];
        dato2 = doc["dato2"];
        
  
        //Serial.println(dato);

// 03
        if (menu != 0){
          String datos_Enviar = "IDplaca=";
          datos_Enviar.concat(IDplaca);
          datos_Enviar.concat("&reset=1");
          peticion("json.php",datos_Enviar);
          Serial.println(menu);
          Serial.println(dato1);
          Serial.println(dato2);
          break;
        }
        }else{
          Serial.println("El servidor no responde");
        }
        
      }else{lcd.setCursor(0,1);lcd.print("    Offline     "); }
   }
//Despues del while
menuPruebas(menu,dato1,dato2);

}

void menuPruebas(int menu, int dato1, int dato2){
  Serial.println("menuPruebas");
       if (menu==1) { IDreceta = dato1; leerReceta();}
  else if (menu==2) { lanzar_Procesos(dato1,dato2);}
  else if (menu==4) { trasvase();}
  
  else Serial.println("La accion deseada no existe-> menuPruebas");
}

void lanzar_Procesos(int proceso, int paso){
       if (proceso==1) { pasoProceso = paso; maceracion();}
  else if (proceso==2) { pasoProceso = paso; coccion();}
  else if (proceso==3) { pasoProceso = paso; fermentacion();}
  else Serial.println("La accion deseada no existe-> lanzar_Procesos");
}
