/*
 * Metodo leer. 
 * Sirve para leer una cadena de caracteres que se recibe de la SQL
 * y se almacena en un String. Los datos vienen separados por ";".
 * 
 * Contiene el parametro del ID de la receta que tiene que leer de la SQL.
 * Asigna los valores oportunos a las variables de cotrol (tmperatura y tiempo de cada proceso).
 */

void leerReceta(){
  if (WiFi.status() == WL_CONNECTED) {

    String data_To_Send = "IDreceta=";
    data_To_Send.concat(Recipe.getRecipe());
    String datos = peticion("getReceta.php", data_To_Send);

    if (datos != "") {
      const size_t capacity = JSON_OBJECT_SIZE(7) + 150;
      DynamicJsonDocument recipe(capacity);

      const char* recipeChar = datos.c_str();
      deserializeJson(recipe, recipeChar);

      String nombre = recipe["nombre"];
      String stempMacer = recipe["tempMacer"];
      String stiempoMacer = recipe["tiempoMacer"];
      String stempCoc = recipe["tempCoc"];
      String stiempoCoc = recipe["tiempoCoc"];
      String stempFermen = recipe["tempFermen"];
      String stiempoFermen = recipe["tiempoFermen"];

      #ifdef debug
      Serial.print("Receta a carga: ");
      Serial.println(data_To_Send);
      Serial.println("------------------------------");
      Serial.print("Json recibido: ");
      Serial.println(datos);
      Serial.println("------------------------------");
      #endif

      for (int i = 0; i < count(stempMacer); i++){
        Recipe.setTempMacer(i, s.separa(stempMacer, ':', i).toInt());
      }
      

      for (int i = 0; i < count(stiempoMacer); i++){
        Recipe.setTimesMacer(i, s.separa(stiempoMacer, ':', i).toInt());
      }
    

      for (int i = 0; i < count(stempCoc);i ++){
        Recipe.setTempCoc(i, s.separa(stempCoc, ':', i).toInt());
      }
      

      for (int i = 0; i < count(stiempoCoc); i++){
        Recipe.setTimesCoc(i, s.separa(stiempoCoc, ':', i).toInt());
      }
      

      for (int i = 0; i < count(stempFermen); i++){
        Recipe.setTempFermen(i, s.separa(stempFermen, ':', i).toInt());
      }
      

      for (int i = 0; i < count(stiempoFermen); i++){
        Recipe.setTimesFermen(i, s.separa(stiempoFermen, ':', i).toInt());
      }

    Recipe.printRecipe();

    }else{
      #ifdef debug
        Serial.println("Error al obtener la receta");
      #endif
    }

  }
}
/*
 * Cuenta los datos que hay en una string separados por ':'
 * Ejemplo String 'pera:manzana:limon' el output de la función es 3
 */
int count(String str){
  
  int cont = 1;
  
  for (int i = 0; i < str.length(); i ++){
  
      if (str[i] == ':') {cont++;}
  
  }

  return cont;

}