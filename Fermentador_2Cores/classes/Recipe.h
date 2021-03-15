class Recipe
{
  private:
  
    #define ARRAYS_LENGHT 10
    int id_Receta;
    String name;
    int tempMacer[ARRAYS_LENGHT];
    int timesMacer[ARRAYS_LENGHT];
    int tempCoc[ARRAYS_LENGHT];
    int timesCoc[ARRAYS_LENGHT];
    int tempFermen[ARRAYS_LENGHT];
    int timesFermen[ARRAYS_LENGHT];
    
  public:

    Recipe() {
      id_Receta = 0;
      name = "No receta";}

    void setRecipe(int number) {id_Receta = number;}
    void setName(String nombre) {name = nombre;}
    void setTempMacer(int index, int data){tempMacer[index] = data;}
    void setTimesMacer(int index, int data){timesMacer[index] = data;}
    void setTempCoc(int index, int data){tempCoc[index] = data;}
    void setTimesCoc(int index, int data){timesCoc[index] = data;}
    void setTempFermen(int index, int data){tempFermen[index] = data;}
    void setTimesFermen(int index, int data){timesFermen[index] = data;}

    int getRecipe() {return (id_Receta);}
    String getName() {return name;}
    int getTempMacer(int index) {return (tempMacer[index]);}
    int getTimeMacer(int index) {return (timesMacer[index]);}
    int getTempCoc(int index) {return (tempCoc[index]);}
    int getTimeCoc(int index) {return (timesCoc[index]);}
    int getTempFermen(int index) {return (tempFermen[index]);}
    int getTimeFermen(int index) {return (timesFermen[index]);}

    void printRecipe();
    void clear();

};

void Recipe::clear() {
  
  id_Receta = 0;

  for (int i = 0; i < ARRAYS_LENGHT; i++){

    tempMacer[i] = 0;
    timesMacer[i] = 0;
    tempCoc[i] = 0;
    timesCoc[i] = 0;
    tempFermen[i] = 0;
    timesFermen[i] = 0;
  }
  

}

void Recipe::printRecipe() {

  Serial.println("Id de la receta cargada");
  Serial.println(id_Receta);

  Serial.println("Nombre de la receta");
  Serial.println(name);

  Serial.println("Temperatura del proceso Maceración");
  for (int i = 0; i < 10; i++)
  {
      Serial.print(tempMacer[i]);
      Serial.print(" "); 
  }
  Serial.println("\n");

  Serial.println("Tiempo en Minutos del proceso Maceración");
  for (int i = 0; i < 10; i++)
  {
      Serial.print(timesMacer[i]); 
      Serial.print(" "); 
  }
  Serial.println("\n");


  Serial.println("Temperatura del proceso Cocción");
  for (int i = 0; i < 10; i++)
  {
      Serial.print(tempCoc[i]); 
      Serial.print(" "); 
  }
  Serial.println("\n");

  Serial.println("Tiempo en Minutos del proceso Cocción");
  for (int i = 0; i < 10; i++)
  {
      Serial.print(timesCoc[i]); 
      Serial.print(" "); 
  }
  Serial.println("\n");


  Serial.println("Temperatura del proceso de Fermentación");
  for (int i = 0; i < 10; i++) {
    Serial.print(tempFermen[i]); 
    Serial.print(" "); 
  }
  Serial.println("\n");

  Serial.println("Tiempo en Meses del proceso Fermentación");
  for (int i = 0; i < 10; i++) {
    Serial.print(timesFermen[i]);
    Serial.print(" ");
  }
  Serial.println("\n");

}