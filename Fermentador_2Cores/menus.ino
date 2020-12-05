/* 
 * El json_menu lo que hace es comprobar si esta conectado a internet y luego recoge datos en formato json,
 * los datos son 3 variables, después resetea los valores que ha cogido de la BDD, por último, ejecuta 
 * la función menu pruebas con los datos recogidos por el json.
 */

// 01
void json_menu(){

  while(true){

    delay(100);
    if (WiFi.status() == WL_CONNECTED){
      #ifdef pantallaLCD
        lcd.setCursor(0,1);
        lcd.print(" Ready   Online");
      #endif
      String datos_Enviar = "IDplaca=";
      datos_Enviar.concat(id_Board);
      String datosString = peticion("json.php", datos_Enviar);

  // 02
    if (datosString == "fallo") {
      
      #ifdef debug
          Serial.println("El servidor no responde");
      #endif
      
      }else {

        const char * datos = datosString.c_str();
      
        const size_t capacity = JSON_OBJECT_SIZE(3) + 30;
        DynamicJsonDocument doc(capacity);

        //const char* json = "{\"menu\":\"1\",\"dato1\":\"1\",\"dato2\":\"1\"}";        //Para testear la decodificación de las variables
        
        deserializeJson(doc, datos);

        int menu = doc["menu"];
        int dato1 = doc["dato1"];
        int dato2 = doc["dato2"];
        

        //Serial.println(dato);

        if (menu != 0){

          String data_To_Send = "IDplaca=";
          data_To_Send.concat(id_Board);
          data_To_Send.concat("&reset=1");
          peticion("json.php", data_To_Send);
            #ifdef debug
              Serial.println(menu);
              Serial.println(dato1);
              Serial.println(dato2);
            #endif
          
          menuPruebas(menu,dato1,dato2);

          break;
        }

      }
          
    }

  }

}

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
  #ifdef debug
    Serial.println("menuPruebas");
  #endif
  switch (menu) {
    case 1:
      Recipe.setRecipe(dato1);
      leerReceta();
    break;

    case 2:
      Serial.println("Lanzar Proceso");
      lanzar_Procesos(dato1,dato2);
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
      Serial.println("Maceración");
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
