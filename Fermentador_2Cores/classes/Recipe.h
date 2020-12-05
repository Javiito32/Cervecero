class Recipe
{
  private:
    int id_Receta;
    int tempMacer[10];
    int timesMacer[10];
    int tempCoc[10];
    int timesCoc[10];
    int tempFermen[10];
    int timesFermen[10];
  public:

    Recipe() {id_Receta = 0;}

    void setRecipe(int number) {id_Receta = number;}
    void setTempMacer(int index, int data){tempMacer[index] = data;}
    void setTimesMacer(int index, int data){timesMacer[index] = data;}
    void setTempCoc(int index, int data){tempCoc[index] = data;}
    void setTimesCoc(int index, int data){timesCoc[index] = data;}
    void setTempFermen(int index, int data){tempFermen[index] = data;}
    void setTimesFermen(int index, int data){timesFermen[index] = data;}

    int getRecipe() {return (id_Receta);}
    int getTempMacer(int index) {return (tempMacer[index]);}
    int getTimeMacer(int index) {return (timesMacer[index]);}
    int getTempCoc(int index) {return (tempCoc[index]);}
    int getTimeCoc(int index) {return (timesCoc[index]);}
    int getTempFermen(int index) {return (tempFermen[index]);}
    int getTimeFermen(int index) {return (timesFermen[index]);}

    void printRecipe();

};

void Recipe::printRecipe() {
      //Temperaturas
        Serial.println("Temperatura del proceso Maceración");
        for (int i = 0; i < 10; i++)
        {
            Serial.print(tempMacer[i]);
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

        Serial.println("Temperatura del proceso de Fermentación");
        for (int i = 0; i < 10; i++)
        {
            Serial.print(tempFermen[i]); 
            Serial.print(" "); 
        }
        Serial.println("\n");

      //Tiempos en segundos
        Serial.println("Tiempo en Minutos del proceso Maceración");
        for (int i = 0; i < 10; i++)
        {
            Serial.print(timesMacer[i]); 
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

        Serial.println("Tiempo en Meses del proceso Fermentación");
        for (int i = 0; i < 10; i++)
        {
            Serial.print(timesFermen[i]); 
            Serial.print(" "); 
        }
        Serial.println("\n");

}