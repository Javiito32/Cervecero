/*
 * Hace las peticiones de datos al servidor con una consulta por el metodo POST
 */
String peticion(String php, String datos_Enviar){
  
  http.begin(host+php);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(datos_Enviar);

  if (httpCode == 200 || httpCode == 201) {

    String datos = http.getString();
    http.end();
    return datos;
  }else{

    http.end();
    return "fallo";
  }
}
/*
 * Hace el log en la BBDD con los datos del proceso
 */

void Log(int id_Board, int id_Recipe, int actualProcess, int stageProcess, int state, int timeLeft, byte percentage) {

  if (WiFi.status() == WL_CONNECTED) {

    String data_To_Send = "IDplaca=";
    data_To_Send.concat(id_Board);
    data_To_Send.concat("&receta=");
    data_To_Send.concat(id_Recipe);
    data_To_Send.concat("&proceso=");
    data_To_Send.concat(actualProcess);
    data_To_Send.concat("&pasoProceso=");
    data_To_Send.concat(stageProcess);
    data_To_Send.concat("&estado=");
    data_To_Send.concat(state);
    data_To_Send.concat("&tiempoRestante=");
    data_To_Send.concat(timeLeft);
    data_To_Send.concat("&porcentaje="); 
    data_To_Send.concat(percentage);
    peticion("log.php", data_To_Send);

    Serial.println("Log");
  }
}

void printLCD(byte linea1, byte posicion1, String datosL1, byte linea2, byte posicion2, String datosL2) {

  lcd.clear();
  lcd.setCursor(posicion1, linea1);
  lcd.print(datosL1);
  lcd.setCursor(posicion2, linea2);
  lcd.print(datosL2);
  
}
