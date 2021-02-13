void json_mqtt_decode(String topic, String datosString){

// 01

  if (datosString == "fallo") {
  
    printLCD(0, 0, "Algo salio mal", 1, 0, "");
  
    }else {

      const char* datos = datosString.c_str();
      
      const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
      DynamicJsonDocument doc(capacity);

      // const char* json = "{\"menu\":\"1\",\"dato1\":\"1\",\"dato2\":\"1\"}";        //Para testear la decodificación de las variables
      
      deserializeJson(doc, datos);

      int menu = doc["menu"];
      int dato1 = doc["dato1"];
      int dato2 = doc["dato2"];

      Serial.println(menu);
      Serial.println(dato1);
      Serial.println(dato2);
      
      menuPruebas(menu,dato1,dato2);
    }

}

void menuPruebas(int menu, int dato1, int dato2){

  Serial.println("menuPruebas");

  if (estado != 1) {

    switch (menu) {
    
      case 1:
        leerReceta(dato1);
      break;

      case 2:
        Serial.println("Lanzar Proceso");
        processCandeled = false;
        lanzar_Procesos(dato1, dato2);
        recovery = 0;
        homeMessage();
      break;

      case 3:
        switch (dato1) {
          case 1:
            time_set();
            break;

          case 2:
            printTime();
            break;

          default:
            Serial.println("La accion no existe-> ajustes menupruebas");
            break;
        }
        break;

      default:
        Serial.println("La accion deseada no existe-> menuPruebas");
      break;
    }

  }else {

    if(menu == 4) processCandeled = true;
    
    
  }
}

void lanzar_Procesos(int proceso, int paso){

  switch (proceso) {
    case 1:
      faseProceso = paso;
      Serial.println("Maceración");
      maceracion();
    break;

    case 2:
      faseProceso = paso;
      coccion();
    break;

    case 3:
      trasvase();
    break;

    default:
      Serial.println("La accion deseada no existe-> lanzar_Procesos");
    break;
  }

  Log(id_Board, Recipe.getRecipe(), procesoActual, faseProceso, estado, tiempoRestante, 100, 0);
}

void homeMessage() {

  #ifdef pantallaLCD
    printLCD(0, 0, "Cervecero v" + currentVersion, 1, 0, Recipe.getName());
  #endif
    Serial.println("------------------------------");
    Serial.println("Ready");
    Serial.println("------------------------------");

}
