/*
 * Revisión de si hay algún procesos que se ha quedado a medias
 */
void checkRecovery(){

  Serial.println("Comprobando Recovery");

  while (true){
    String data_To_Send = "IDplaca=";
    data_To_Send.concat(id_Board);
    String datos = peticion("recovery.php", data_To_Send);

      if (datos != "fallo") {

        const size_t capacity = JSON_OBJECT_SIZE(5) + 70;
        DynamicJsonDocument doc(capacity);

        const char* json = datos.c_str();

        deserializeJson(doc, json);

        byte estado = doc["estado"];

        if(estado == 1){

          Recipe.setRecipe(doc["receta"]);
          recoveryTiempoRestante = doc["tiempoRestante"];
          recoveryProceso = doc["proceso"];
          recoveryPasoProceso = doc["pasoProceso"];

          recovery = 1;

          Serial.println("------------------------------");
          Serial.println("Hay procesos pendiantes");
          Serial.println("------------------------------");

          Serial.print("Receta a recuperar: ");
          Serial.println(Recipe.getRecipe());
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

        #ifdef pantallaLCD
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el Recovery");
        #endif
      }
  }
}
/*
 * Lanza el proceso por el que iba en caso de no concluir
 */
void recoveryProcesos(int proceso){
  switch (proceso){
  case 1:

    maceracion();
    break;
  
  case 2:

    coccion();
    break;

  case 3:

    trasvase();
    break;

  case 4:

    //fermentacion();
    break;
  
  default:

    Serial.println("Proceso no existente");

    printLCD(0, 0, "    Proceso", 1, 0, "  no existente");
    break;

  }

  recovery = 0;
  homeMessage();

}
