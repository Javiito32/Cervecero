/*
 * PROGRAMA PARA EL MANEJO DE UN FERMENTADOR
 * 
 * Notas de la version 2:
 *  - Estructura del programa.
 *  - Funcion triaje() para diferenciar procesos.
 *  - Programacion de la maceracion.
 *  - Establecimiento de la consigna de comunicacion.
 * 
 * Notas de la version 3:
 *  - Cambio en la consigna de comunicacion.
 *  - Modificar los pines del arduino y las variables configurables a const 
 *    (constantes durante todo el programa) para dar mas robustez al codigo.
 *  - Añadir comentarios en el setup.
 *  - Añadir puesta a cero inicial de las variables de trabajo.
 *  - Incluir y comentar el metodo leer().
 *  - Incluir y comentar el metodo desencriptarTemperatura().
 *  - Incluir y comentar el metodo desencriptarTiempo().
 * Notas de la version 4:
 *  - Cambio y comentarios en los pines.
 *  - Cambio de las variables de tiempo a float.
 *  - Modificacion del metodo de maceracion. Acabado completamente.
 *  - Programacion del metodo de coccion. Acabado completamente.
 *  - Metodo enviarTiempo(). Acabado completamente.
 *  - Metodo finProceso(). Acabadocompletamente.
 *  
 *  Notas de la version 4.1:
 *  - Hemos añadido los pines de reserva en el LAYOUT de pines.
 *  
 *  Notas de la version 5:
 *  - Metodo menu(). Acabado completamente.
 *  - Metodo recircular(). Acabado completamente.
 *  - Metodo calentar(). Acabado completamente.
 *  - Metodo trasvase(). Acabado completamente.
 *  - Metodo fermentacion(). Falta programarlo.
 *  - Modificacion desencriptarTemperatura(). Daba fallo y 
 *    se han modificado los indices del substring.
 *  - Modificacion desencriptarTiempo(). Daba fallo y 
 *    se han modificado los indices del substring.
 *    
 *  Notas de la version 5.1:
 *  - Creacion de variables cronometro para contar segundos.
 *  - Metodo fermentacion(). Acabado completamente.
 *  - Metodo trasvase(). Añadimos tiempo maximo de seguridad.
 *  
 *  Notas de la version 6:
 *  - Comentar todos los metodos completamente.
 *  - Modificacion desencriptarTemperatura(). Daba fallo y 
 *    se han modificado los indices del substring.
 *    (Han dado varios fallos, si da un fallo en todos los procesos puede venir de aqui)
 *  - Modificacion desencriptarTiempo(). Daba fallo y 
 *    se han modificado los indices del substring.
 *    (Han dado varios fallos, si da un fallo en todos los procesos puede venir de aqui)
 *  - Retraso en el arranque de las bombas para el trasvase.
 *  - Añadir pin para el zumbador
 *  
 *  Notas para la version 7: 
 *  - Metodo cancion1(zumbador). Cancion de piratas del caribe.
 *    Para añadir mas canciones usar la misma estructura: Funcion con la cancion cuyo 
 *    parametro sea el pin del zumbador para facilitar la modificacion de pines en un futuro.
 *    FALTA AÑADIR LA CANCION DESPUES DE LOS PROCESOS QUE SE QUIERA.
 *  - Se ajusta el tiempo de trasvase a 3 minutos y medio segun prueba en maqueta
 *  
 *  Notas de la version 7.1:
 *  - Eliminacion de la variable tsensor
 *  - Modificacion desencriptarTemperatura(). Daba fallo y 
 *    se han modificado los indices del substring.
 *    (Han dado varios fallos depende del sistema operativo o versiones del programa)
 *  - Modificacion desencriptarTiempo(). Daba fallo y 
 *    se han modificado los indices del substring.
 *    (Han dado varios fallos depende del sistema operativo o versiones del programa)
 *
 *  Notas de la version 8:
 *  - Actualizacion de pines para el NodeMCU.
 *  - Eliminacion de electrovalvulas y bomba principal, por Bomba Recirculacion y por
 *    Bomba Trasvase.
 *  - Se ha renombrado el pin reserva1 por peltier, para las Celulas Peltier.
 *  - Se ha renombrado el pin reserva2 por reserva, para tener un pin de reserva.
 *  - En la ecuacion de temperatura hemos cambiado el valor del voltaje de referencia
 *    de 5000mV (Arduino) a 3300mV (NodeMCU).
 */

/*
 * PINES Y VARIABLES
 */
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClientSecureBearSSL.h>
  #include <Wire.h>                           // incluye libreria para interfaz I2C
  #include <RTClib.h>                         // incluye libreria para el manejo del modulo RTC
  #include <TimeLib.h>
//LAYOUT Pines
  const int pinSonda = A0;                    //Sonda de la temperatura
  const int resis = D0;                       //Resistencia para calentar               
  const int bombaRecirculacion = D3;          //Bomba de recirculacion 230V
  const int bombaTrasvase = D4;               //Bomba trasvase 230V
  const int bombaFrio = D5;                   //Bomba refrigeracion 230V
  const int peltier = D6;                     //Celulas Peltier
  const int reserva = D7;                     //Este cable se deja en reserva, era para el RELE DE ESTADO SOLIDO que no se usa
  
//Variables configurables
  const float anchoVentana = 1;               //Rango para la temperatura
  const float tiempoTrasvase = 210000;        //Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;             //Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables de trabajo
  const char* ssid = "";                      //Nombre de la red WiFi a la que se va a conectar
  const char* password = "";                  //Contraseña de la red WiFi a la que se va a conectar
  int sensorTemperatura;                      //Variable que almacen la lectura de la sonda
  int dato;                                   //Dato leido para entrar el menu
  float tmax;                                 //Temperatura maxima para los procesos
  float tmin;                                 //Temperatura minima para los procesos
  unsigned long tiempoi;                      //Tiempo inicial para los procesos en seg
  unsigned long tiempof;                      //Tiempo final para los procesos en seg
  unsigned long tiempoActual;                 //Tiempo actual del proceso en seg
  long tiempoRestante;                        //Tiempo que falta para el final de los procesos en seg
  float tiempoEnviar;                         //Tiempo que se envia a la Rasberry en segundos
  float milivoltios;                          //Variable para almacenar los milivoltios
  float celsius;                              //Variable para almacenar los grados
  //int cronometroi;                            //Tiempo inicial para el envio a Rasberry
  //int cronometrof;                            //Tiempo final pare el envio a Rasberry
  //int cronometro;                             //Tiempo actual para el envio a Rasberry
  float tempMacer;                            //Temperatura de maceración de la receta seleccionada
  unsigned long tiempoMacer;                  //Tiempo maceración de la recta selecionada
  float tempCoc;                              //Temperatura de cocción de la receta seleccionada
  unsigned long tiempoCoc;                    //Tiempo cocción de la recta selecionada
  unsigned long tiempoTrans;                  //Tiempo transvase de la recta selecionada
  float tempFermen;                           //Temperatura de fermentación de la receta seleccionada
  unsigned long tiempoFermen;                 //Tiempo fermentación de la recta selecionada
  unsigned long timeset;                      //Se usa para establecer el tiempo en el RTC
  bool falloProceso = 0;
  //const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};

//Objetos
  HTTPClient http;                            // Object of the class HTTPClient.
  RTC_DS3231 rtc;                             // crea objeto del tipo RTC_DS3231

/*
 * CICLO DE ARRANQUE
 * Configuramos los pines. Puesta a cero inicial.
 */
void setup(){
  
//Inicializamos el puerto serie
  Serial.begin(115200);
  delay(10);
  
//Iniciamos la comunicación con el RTC
  Wire.begin(D2,D1);
  
// Conectar con la red WiFi
  Serial.println("");
  Serial.print("Connecting");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {       //Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());               //Mostrar la IP que tiene el dispositivo
  
//Configuracion de pines
  pinMode(resis,OUTPUT);
  pinMode(bombaRecirculacion,OUTPUT);
  pinMode(bombaTrasvase,OUTPUT);
  pinMode(bombaFrio,OUTPUT);
  pinMode(peltier,OUTPUT);
  pinMode(reserva,OUTPUT);
  
//Puesta a cero inicial de las variables de trabajo
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  tiempoRestante = 0;
  //cronometroi = 0;
  //cronometrof = 0;
  //cronometro = 0;
}
/*
 * CICLO PRINCIPAL
 * Indicamos a la Raspberry que hemos arrancado. 
 * Leemos datos en bucle hasta entrar en un proceso.
 */

void loop(){
  
//Mensaje inicial
  Serial.println("------------------------------");
  Serial.println("Ready");
  Serial.println("------------------------------");
  pregunta();
  menuinicio(dato);
}
void pregunta(){
  while(true){
  delay(100);
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin(*client, "https://192.168.1.150/arduino/menu.php");  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode > 0) {
        String datoString = http.getString();
        dato = datoString.toInt();
        //Serial.println(dato);
        http.end();
        if (dato != 0){
          http.begin(*client, "https://192.168.1.150/arduino/menu.php?reset=1");
          http.GET();
          http.end();
          break;
        }
        }
      }
  }
}

/*
 * Funcion menu.
 * Es el encargado de gestionar el numero de proceso, del 1 al 9, y 
 * elegir el proceso a realizar.
 * Datos del menu:
 *  - 1: Maceracion.
 *  - 2: Coccion.
 *  - 3: Trasvase.
 *  - 4: Fermentacion.
 *  
 *  Parametros: numero de proceso introducido como caracter
 *  No devuelve nada
 */
void menuinicio(int n){ 
       if (n==1) { receta();}
  else if (n==2) { ajustes();}
  else if (n==3) { preparar();}
  else Serial.println("La accion deseada no existe");
}

void receta(){
  Serial.println("Selecciona receta: ");
  pregunta();
  leerdatos(dato);
}

void ajustes(){
  Serial.println("Ajustes");
  pregunta();
       if (dato==1) { time_set();}
  else if (dato==2) { showtime();}
  else Serial.println("La accion deseada no existe");
}

void preparar(){
  Serial.println("Selecciona proceso: ");
  pregunta();
       if (dato==1) { maceracion(); }
  else if (dato==2) { coccion();}
  else if (dato==3) { trasvase();}
  else if (dato==4) { fermentacion();}
  else Serial.println("Proceso no existente");
}

void showtime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
  Serial.println(tiempoActual);
  DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
            // DateTime y asigna a variable fecha
        Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
        Serial.print(" ");       // caracter espacio en blanco como separador
        Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
}

void gettime(){
  DateTime now = rtc.now();
  tiempoActual = now.unixtime();
}

void time_set (){
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin("https://192.168.1.150/arduino/time.php");  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode > 0) {
        String stringtime = http.getString();
        http.end();
        timeset = (long) strtol(stringtime.c_str(),NULL,0);
        rtc.adjust(DateTime(year(timeset),month(timeset),day(timeset),hour(timeset),minute(timeset),second(timeset)));
        DateTime fecha = rtc.now();      // funcion que devuelve fecha y horario en formato
            // DateTime y asigna a variable fecha
        Serial.print(fecha.day());     // funcion que obtiene el dia de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.month());     // funcion que obtiene el mes de la fecha completa
        Serial.print("/");       // caracter barra como separador
        Serial.print(fecha.year());      // funcion que obtiene el año de la fecha completa
        Serial.print(" ");       // caracter espacio en blanco como separador
        Serial.print(fecha.hour());      // funcion que obtiene la hora de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.print(fecha.minute());      // funcion que obtiene los minutos de la fecha completa
        Serial.print(":");       // caracter dos puntos como separador
        Serial.println(fecha.second());    // funcion que obtiene los segundos de la fecha completa
      }
  }
 }


/*  
 *  Funcion para realizar MACERACION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  La consigna empieza por "T", seguida de cuatro numeros (centenas de temperatura, decenas de 
 *  temperatura, unidades de temperatura y decimas de temperatura), seguidas por "S" y el tiempo
 *  del proceso en segundos, acabando la consigna en "."
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la constante.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */

  
void maceracion (){
//Confirmacion para RASPBERRY del inicio de proceso de maceracion
  Serial.println("O1");
  
//LECTURA DE VARIABLES
  //String informacionMaceracion = leer();
  //float temperaturaMaceracion = desencriptarTemperatura (informacionMaceracion);      //Variable con la temperatura del proceso
  //float tiempoMaceracion = desencriptarTiempo (informacionMaceracion);                //Variable del tiempo del proceso en segundos
  float temperaturaMaceracion = tempMacer;
  float tiempoMaceracion = tiempoMacer;
//MODO RECIRCULACION
  recircular();
  
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
  
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(1,falloProceso);

  
  
}

/*
 *  Funcion para realizar COCCION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  La consigna empieza por "T", seguida de cuatro numeros (centenas de temperatura, decenas de 
 *  temperatura, unidades de temperatura y decimas de temperatura), seguidas por "S" y el tiempo
 *  del proceso en segundos, acabando la consigna en "."
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la constante.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void coccion (){
//Confirmacion para RASPBERRY del inicio de proceso de maceracion
  Serial.println("O2");
  
//LECTURA DE VARIABLES
  //String informacionCoccion = leer();
  //float temperaturaCoccion = desencriptarTemperatura (informacionCoccion);      //Variable con la temperatura del proceso
  //float tiempoCoccion = desencriptarTiempo (informacionCoccion);                //Variable del tiempo del proceso en segundos
  float temperaturaCoccion = tempCoc;
  float tiempoCoccion = tiempoCoc;

//MODO RECIRCULACION
  recircular();

//CICLO DE CALENTAMIENTO
  calentar(temperaturaCoccion, tiempoCoccion);
  
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(2,0);
}

/*
 *  Funcion para realizar TRASVASE.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso y activa las 
 *  bombas y reles necesarios.
 *  Se pone en modo trasvase hasta que recibe un mensaje de fin que viene dado por la 
 *  siguiente consigna: "T0000S0."
 *    
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void trasvase(){
  boolean fin = false;
//Confirmacion para RASPBERRY del inicio de proceso de trasvase
  Serial.println("O3");
  
//Iniciamos el tiempo
  gettime();
  
//Trasvase ON
  digitalWrite(bombaFrio,HIGH);
  delay(retrasoBombas);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(peltier,HIGH);
  
//Lectura de orden de fin
  do{
  //Control del proceso por tiempo limite de seguridad
    tiempoActual = millis();
    tiempoRestante = tiempoActual - tiempoi;                                        //Tiempo que llevamos de trasvase
    if(tiempoRestante > tiempoTrasvase){fin = true;}
  //Control del proceso por orden de la Raspberry
    //String informacionTrasvase = leer();
    //float tiempoTrasvase = desencriptarTiempo (informacionTrasvase);                //Variable del tiempo del proceso en segundos
    float tiempoTrasvase = tiempoTrans;
    if(tiempoTrasvase == 0.00){fin = true;}
  }while(!fin);
  
//Trasvase OFF  
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
  
//Puesta a cero de las variables
  tiempoi = 0;
  tiempoActual = 0;
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(3,0);
}



/*
 *  Funcion para realizar FERMENTACION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  Finaliza el proceso cuando acaba el tiempo
 *  Encripta las variables y las envia.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void fermentacion(){
//Confirmacion para RASPBERRY del inicio de proceso de fermentacion
  Serial.println("O4");
  
//LECTURA DE VARIABLES
  //String informacionFermentacion = leer();
  //float temperaturaFermentacion = desencriptarTemperatura (informacionFermentacion);      //Variable con la temperatura del proceso
  //float tiempoFermentacion = desencriptarTiempo (informacionFermentacion);                //Variable del tiempo del proceso en segundos
  float temperaturaFermentacion = tempFermen;
  long tiempoFermentacion = tiempoFermen;

    boolean fin = false;
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoFermen * 2628000);
    do{
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      Serial.print(day(tiempoRestante));     // funcion que obtiene el dia de la fecha completa
      Serial.print(" dias /");       // caracter barra como separador
      Serial.print(hour(tiempoRestante));      // funcion que obtiene la hora de la fecha completa
      Serial.print(":");       // caracter dos puntos como separador
      Serial.println(minute(tiempoRestante));      // funcion que obtiene los minutos de la fecha completa
      delay(60000);
    }while(!fin);
//PUESTA A CERO FINAL
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  tiempoEnviar = 0;
  //cronometro = 0;
  //cronometroi = 0;
  //cronometrof = 0;
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(4,0);
}


/*
 * Metodo para recircular. 
 * Sirve poner los reles y bombas en la posicion de recirculacion.
 * 
 * No tiene parametros
 * No devuelve nada
 */
void recircular(){
  digitalWrite(bombaRecirculacion,HIGH);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
}

/*
 * Metodo calentar. 
 * Sirve realizar un ciclo de calentamiento hasta la temperatura alcanzar la temperatura pasada como parametro
 * Parametros: tiempo del proceso en segundos
 *             temperatura del proceso en grados
 * No devuelve nada
 */
void calentar( float temperaturaProceso, long tiempoProceso){
//TRATAMIENTO DE LAS VARIABLES
  //Tratamiento de la ventana de temperatura
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(tiempoProceso);
    Serial.println(" Minutos");
    Serial.println("------------------------");
    tmax = temperaturaProceso+anchoVentana;
    tmin = temperaturaProceso-anchoVentana;
    boolean fin = false;
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoProceso * 60);
    long comprobarCancelacion;
    comprobarCancelacion = tiempoActual + 5;
    do{
      if (tiempoActual >= comprobarCancelacion){
        comprobarCancelacion = tiempoActual + 5;
        cancelar();
        if (falloProceso){
          break;
        }
      }
      
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      //enviarTiempo(tiempoRestante);
      Serial.print("Quedan ");
      Serial.print(minute(tiempoRestante));
      Serial.print(" Min y ");
      Serial.print(second(tiempoRestante));
      Serial.println(" Segundos");
  //Tratamiento de la temperatura
    sensorTemperatura = analogRead(pinSonda);
    milivoltios = (sensorTemperatura / 1023.0) * 3300;
    celsius = milivoltios / 10;
 
  //Mantenimiento de la ventana de temperatura
    if(celsius > tmax){digitalWrite(resis,LOW);}
    if(celsius < tmin){digitalWrite(resis,HIGH);}
    if (tiempoRestante <= 0) fin = true;
    delay(1000);
  }while(!fin);
  
//PUESTA A CERO FINAL
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  //cronometro = 0;
  //cronometroi = 0;
  //cronometrof = 0;
}

void cancelar() {
  if (WiFi.status() == WL_CONNECTED){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin("https://192.168.1.150/arduino/fallo.php");  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode > 0) {
        String stringcancelar = http.getString();
        int cancelar = stringcancelar.toInt();
        if (cancelar == 1){
          falloProceso = 1;
          http.end();
        }
    }
   }
}

/*
 * Metodo leer. 
 * Sirve para leer una cadena de caracteres que se recibe de la SQL
 * y se almacena en un String. Los datos vienen separados por ";".
 * 
 * Contiene el parametro del ID de la receta que tiene que leer de la SQL.
 * Asigna los valores oportunos a las variables de cotrol (tmperatura y tiempo de cada proceso).
 */

void leerdatos(int n){
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;  // Object of the class HTTPClient.
    String ppeticion = "http://192.168.1.150/arduino/pedirdatos.php?id=";
    String peticion = ppeticion + n;
    Serial.println("------------------------------");
    Serial.print("Petición al servidor: ");
    Serial.println(peticion);
    http.begin(peticion);  // Request destination.
    int httpCode = http.GET(); // Send the request.

    if (httpCode > 0) { //Check the returning code

      String datos = http.getString(); 
      Serial.println("------------------------------");                           // Obtiene la string
      Serial.print("String recibida: ");
      Serial.println(datos);
      int longitud = datos.length();
      Serial.println("------------------------------");

    //Procesar datos nombre Receta
      int pnombre = datos.indexOf("nombre=");
      String nombre = "";
      for (int i = pnombre + 7; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else nombre += datos[i];
      }
      

    //Procesar datos de la Temperatura de Maceración
      int ptempMacer = datos.indexOf("tempMacer=");               //Posicion de temp
      String stempMacer = "";
      for (int i = ptempMacer + 10; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempMacer += datos[i];
      }
      tempMacer = stempMacer.toFloat();
      

    //Procesar datos tiempo la Maceración
      int ptiempoMacer = datos.indexOf("tiempoMacer=");
      String stiempoMacer = "";
      for (int i = ptiempoMacer + 12; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoMacer += datos[i];
      }
      tiempoMacer = (long) strtol(stiempoMacer.c_str(),NULL,0);
      

    //Procesar datos de la Temperatura de Cocción
      int ptempCoc = datos.indexOf("tempCoc=");
      String stempCoc = "";
      for (int i = ptempCoc + 8; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempCoc += datos[i];
      }
      tempCoc = stempCoc.toFloat();
      

    //Procesar datos tiempo de Cocción
      int ptiempoCoc = datos.indexOf("tiempoCoc=");
      String stiempoCoc = "";
      for (int i = ptiempoCoc + 10; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoCoc += datos[i];
      }
      tiempoCoc = (long) strtol(stiempoCoc.c_str(),NULL,0);
      

    //Procesar datos tiempo del Transbase
      int ptiempoTrans = datos.indexOf("tiempoTrans=");
      String stiempoTrans = "";
      for (int i = ptiempoTrans + 12; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoTrans += datos[i];
      }
      tiempoTrans = (long) strtol(stiempoTrans.c_str(),NULL,0);
      

    //Procesar datos tiempo del Fermentación
      int ptempFermen = datos.indexOf("tempFermen=");
      String stempFermen = "";
      for (int i = ptempFermen + 11; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stempFermen += datos[i];
      }
      tempFermen = stempFermen.toFloat();
      

    //Procesar datos de la Temperatura de Fermentación
      int ptiempoFermen = datos.indexOf("tiempoFermen=");
      String stiempoFermen = "";
      for (int i = ptiempoFermen + 13; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoFermen += datos[i];
      }
      tiempoFermen = (long) strtol(stiempoFermen.c_str(),NULL,0);
      

    //Mostrar información de la receta por Serial
    if (tiempoMacer != 0){
      //Nombre de la cerveza
        Serial.print("Nombre de la cerveza= ");
        Serial.println(nombre);
      //Temperaturas
        Serial.print("Temperatura del proceso Maceración= ");
        Serial.println(tempMacer);
        Serial.print("Temperatura del proceso Cocción= ");
        Serial.println(tempCoc);
        Serial.print("Temperatura del proceso de Fermentación= ");
        Serial.println(tempFermen);
      //Tiempos en segundos
        Serial.print("Tiempo en Minutos del proceso Maceración= ");
        Serial.println(tiempoMacer);
        Serial.print("Tiempo en Segundos del proceso Cocción= ");
        Serial.println(tiempoCoc);
        Serial.print("Tiempo en Segundos del proceso Transbase= ");
        Serial.println(tiempoTrans);
        Serial.print("Tiempo en Meses del proceso Fermentación= ");
        Serial.println(tiempoFermen);
    }else{
      Serial.println("La receta no existe");
    }
    }else{

      Serial.println("Error de conexión");

    }

    http.end();   //Close connection
}
}



/*
 * Metodo para enviar el final del proceso con errores.
 * Envia un mensaje a la Rasberry con los errores del proceso.
 * 
 * Parametros: dato  Representa el numero de proceso
 *             error Representa el numero de error (0 si no hay)
 * No devuelve nada
 */
void finProceso (int proceso,bool error){
//Variables locales
  String mensaje = "O";
  
//Conversion a String
  mensaje.concat(proceso);
  mensaje.concat("F");
  mensaje.concat(error);
  if (falloProceso){
    if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin(*client, "https://192.168.1.150/arduino/fallo.php?reset=1");
    http.GET();
    http.end();
  }
  falloProceso = 0;
 }
  
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}
