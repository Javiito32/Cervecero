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
  else {
    #ifdef debug
    Serial.println("La accion deseada no existe");
    #endif
  }
}

void receta(){
  #ifdef debug
  Serial.println("------------------------------");
  Serial.println("Selecciona receta: ");
  Serial.println("------------------------------");
  #endif
  SQL_menu();
  IDreceta = dato;
  leerReceta();
}

void procesos(){
  if (IDreceta == 0){
    #ifdef debug
    Serial.println("Primero selecciona una receta");
    #endif
    return;
  }
  #ifdef debug
  Serial.println("Selecciona proceso: ");
  #endif
  SQL_menu();
       if (dato==1) { SQL_menu(); faseProceso = dato; maceracion(); }
  else if (dato==2) { SQL_menu(); faseProceso = dato; coccion();}
  else if (dato==3) { trasvase();}
  else if (dato==4) { SQL_menu(); faseProceso = dato; fermentacion();}
  else {
    #ifdef debug
    Serial.println("Proceso no existente");
    #endif
  }
}


void limpieza(){
  #ifdef debug
    Serial.println("------------------------");
    Serial.println("Se ha iniciado la limpieza");
    Serial.println("    Por favor, espere");
    Serial.println("------------------------");
  #endif
    trasvase();
}

void ajustes(){
  #ifdef debug
    Serial.println("Ajustes");
  #endif

  SQL_menu();

  switch (dato) {
    case 1:
      time_set();
    break;

    case 2:
      printTime();
    break;

    default:
      #ifdef debug
        Serial.println("La accion deseada no existe");
      #endif
    break;
  }
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
    #ifdef pantallaLCD
      lcd.setCursor(0,1);
      lcd.print(" Ready   Online");
    #endif
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
            #ifdef debug
              Serial.println(menu);
              Serial.println(dato1);
              Serial.println(dato2);
            #endif
          break;
        }
        }else{
          #ifdef debug
            Serial.println("El servidor no responde");
          #endif
        }
        
      }else {
        #ifdef pantallaLCD
          lcd.setCursor(0,1);
          lcd.print("    Offline     ");
        #endif
        }
   }
//Despues del while
menuPruebas(menu,dato1,dato2);

}

void menuPruebas(int menu, int dato1, int dato2){
  #ifdef debug
  Serial.println("menuPruebas");
  #endif
  switch (menu) {
    case 1:
      IDreceta = dato1; 
      leerReceta();
    break;

    case 2:
      lanzar_Procesos(dato1,dato2);
    break;

    case 3:
      switch (dato1) {
        case 1:
          time_set ();
        break;

        case 2:
          printTime();
        break;

        default:
          #ifdef debug
            Serial.println("La accion no existe-> ajustes menupruebas");
          #endif
        break;
      }
    break;

    default:
      #ifdef debug
        Serial.println("La accion deseada no existe-> menuPruebas");
      #endif 
    break;
  }
}

void lanzar_Procesos(int proceso, int paso){
  switch (proceso) {
    case 1:
      faseProceso = paso;
      maceracion();
    break;

    case 2:
      faseProceso = paso;
      coccion();
    break;

    case 3:
      faseProceso = paso;
      fermentacion();
    break;

    case 4:
      trasvase();
    break;

    default:
      #ifdef debug
        Serial.println("La accion deseada no existe-> lanzar_Procesos");
      #endif
    break;
  }
}
