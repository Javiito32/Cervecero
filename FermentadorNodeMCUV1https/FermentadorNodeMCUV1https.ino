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
  #include <Separador.h>
  
//LAYOUT Pines
  #define pinSonda A0                    //Sonda de la temperatura
  #define resis D0                       //Resistencia para calentar               
  #define bombaRecirculacion D3          //Bomba de recirculacion 230V
  #define bombaTrasvase D4               //Bomba trasvase 230V
  #define bombaFrio D5                   //Bomba refrigeracion 230V
  #define peltier D6                     //Celulas Peltier
  #define sensorLiquido D7               //Sensor de liquido en tubo
  #define zumbador D8                    //Zumbador para reproducir canciones
  
//Variables configurables
  const float anchoVentana = 1;               //Rango para la temperatura
  const float tiempoTrasvase = 210000;        //Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;             //Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables globales
  const char* ssid = "";                      //Nombre de la red WiFi a la que se va a conectar
  const char* password = "";                  //Contraseña de la red WiFi a la que se va a conectar
  int dato;                                   //Dato leido para entrar el menu
  unsigned long tiempoi;                      //Tiempo inicial para los procesos en seg
  unsigned long tiempof;                      //Tiempo final para los procesos en seg
  unsigned long tiempoActual;                 //Tiempo actual del proceso en seg
  long tiempoRestante;                        //Tiempo que falta para el final de los procesos en seg
  String tempMacer[10];                       //Temperatura de maceración de la receta seleccionada
  String tiempoMacer[10];                     //Tiempo maceración de la recta selecionada
  float tempCoc;                              //Temperatura de cocción de la receta seleccionada
  unsigned long tiempoCoc;                    //Tiempo cocción de la recta selecionada
  unsigned long tiempoTrans;                  //Tiempo transvase de la recta selecionada
  float tempFermen;                           //Temperatura de fermentación de la receta seleccionada
  unsigned long tiempoFermen;                 //Tiempo fermentación de la recta selecionada
  bool falloProceso = 0;                      //Guarda si falla el tiempo
  unsigned char procesoActual;
  unsigned char estado;
  String mac;
  int IDplaca;
  int IDreceta;
  byte porcentaje;
  bool recovery;
  int recoveryTiempoRestante;
  int recoveryProceso;
  int recoveryPasoProceso;
  
  //const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
    
//Objetos
  HTTPClient http;                              // Object of the class HTTPClient.
  RTC_DS3231 rtc;                               // crea objeto del tipo RTC_DS3231
  Separador s;                                  //Objeto para separar datos

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

//Configuracion de pines
  pinMode(resis,OUTPUT);
  pinMode(bombaRecirculacion,OUTPUT);
  pinMode(bombaTrasvase,OUTPUT);
  pinMode(bombaFrio,OUTPUT);
  pinMode(peltier,OUTPUT);
  pinMode(sensorLiquido,INPUT);
  pinMode(zumbador,OUTPUT);
  
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
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  mac = WiFi.macAddress();

//Solicitud de Identificador de placa según la mac
  while (true){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/get_id.php?mac=";
    consulta = consulta + mac;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String stringIDplaca = http.getString();
        http.end();
        IDplaca = stringIDplaca.toInt();
        Serial.println("------------------------------");
        Serial.print("El ID de la placa es el: ");
        Serial.println(IDplaca);
        Serial.println("------------------------------");
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No hay ningún recovery");
        Serial.println("------------------------------");
      }
  }
  while (true){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/recovery.php?IDplaca=";
    consulta = consulta + IDplaca;
    //Serial.println(consulta);
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datos = http.getString();
        Serial.println(datos);
        http.end();
        //Decode
        int longitud = datos.length();
        int pestado = datos.indexOf("estado=");
        String sestado = "";
        for (int i = pestado + 7; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else sestado += datos[i];
        }
        int estado = sestado.toInt();

        if(estado == 1){
          recovery = 1;
          Serial.println("------------------------------");
          Serial.println("Iniciando proceso de recovery");
          Serial.println("------------------------------");


          int preceta = datos.indexOf("receta=");
          String sreceta = "";
          for (int i = preceta + 7; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sreceta += datos[i];
          }
          IDreceta = sreceta.toInt();
          

          int ptiempoRestante = datos.indexOf("tiempoRestante=");
          String stiempoRestante = "";
          for (int i = ptiempoRestante + 15; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else stiempoRestante += datos[i];
          }
          recoveryTiempoRestante = stiempoRestante.toInt();



          int pproceso = datos.indexOf("proceso=");
          String sproceso = "";
          for (int i = pproceso + 8; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else sproceso += datos[i];
          }
          recoveryProceso = sproceso.toInt();



          int ppasoProceso = datos.indexOf("pasoProceso=");
          String spasoProceso = "";
          for (int i = ppasoProceso + 12; i < longitud; i ++){
          if (datos[i] == ';') i = longitud;
          else spasoProceso += datos[i];
          }
          recoveryPasoProceso = spasoProceso.toInt();


     
        }
        Serial.println("Receta a recuperar: ");
        Serial.print(IDreceta);
        Serial.println("Tiempo que le falta: ");
        Serial.print(recoveryTiempoRestante);
        Serial.println("Proceso que estaba: ");
        Serial.print(recoveryProceso);
        Serial.println("Paso del proceso que estaba: ");
        Serial.print(recoveryPasoProceso);
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No hay recovery");
        Serial.println("------------------------------");
      }
  }
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
    String consulta = "https://192.168.1.150/arduino/menu.php?menu=1&IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client, consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String datoString = http.getString();
        http.end();
        dato = datoString.toInt();
        //Serial.println(dato);
        if (dato != 0){
          String consulta = "https://192.168.1.150/arduino/menu.php?resetmenu=1&IDplaca=";
          consulta = consulta + IDplaca;
          http.begin(*client, consulta);
          http.GET();
          http.end();
          break;
        }
        }else{
          Serial.println("El servidor no responde");
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
  else if (n==2) { procesos();}
  else if (n==3) { ajustes();}
  else if (n==4) { limpieza();}
  else Serial.println("La accion deseada no existe");
}

void receta(){
  Serial.println("------------------------------");
  Serial.println("Selecciona receta: ");
  Serial.println("------------------------------");
  pregunta();
  IDreceta = dato;
  leerReceta();
}

void procesos(){
  if (tempMacer == 0){
    Serial.println("Primero selecciona una receta");
    return;
  }
  Serial.println("Selecciona proceso: ");
  pregunta();
       if (dato==1) { pregunta(); maceracion(dato); }
  else if (dato==2) { pregunta(); coccion(dato);}
  else if (dato==3) { trasvase();}
  else if (dato==4) { pregunta(); fermentacion(dato);}
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
  pregunta();
       if (dato==1) { time_set();}
  else if (dato==2) { showtime();}
  else if (dato==3) { tonos();}
  else if (dato==4) { pruebas();}
  
  else Serial.println("La accion deseada no existe");
}

void pruebas(){
  Serial.println(month(1597976328));
  Serial.println(day(1597976328));
}

void tonos(){
  Serial.println("------------------------------");
  Serial.println("Selecciona Música: ");
  Serial.println("------------------------------");
  pregunta();
       if (dato==1) { cancion2(); }
  else if (dato==2) { cancion1();}
  else if (dato==3) { c_nokia_c();}
  else Serial.println("La canción no existe");
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
      if (httpCode == 200 || httpCode == 201) {
        String stringtime = http.getString();
        http.end();
        unsigned long timeset = (long) strtol(stringtime.c_str(),NULL,0);
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

  
void maceracion (byte pasoProceso){
  if(recovery == 1){
    procesoActual = 1;
    estado = 1;
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[pasoProceso].toFloat();           //Variable con la temperatura del proceso
  int tiempoMaceracion = tiempoMacer[pasoProceso].toInt();                  //Variable del tiempo del proceso en minutos
//MODO RECIRCULACION
  recircular();
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
  
  }else{
//Confirmacion del inicio de proceso de maceracion
  Serial.println("O1");
//Configuracion del proceso
  procesoActual = 1;
  estado = 1;
  sendInfo(procesoActual,pasoProceso,estado);
//LECTURA DE VARIABLES
  float temperaturaMaceracion = tempMacer[pasoProceso].toFloat();           //Variable con la temperatura del proceso
  int tiempoMaceracion = tiempoMacer[pasoProceso].toInt();                  //Variable del tiempo del proceso en minutos
//MODO RECIRCULACION
  recircular();
  
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
}
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
  
//Envio mensaje de fin de proceso.
  if (falloProceso) estado = 3;
  else {estado = 2; c_nokia_c();};
  recovery = 0;
  sendInfo(procesoActual,pasoProceso,estado);
  finProceso(procesoActual,falloProceso);
  
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
void coccion (byte pasoProceso){ 
//Confirmacion del inicio de proceso de cocción
  Serial.println("O2");
  procesoActual = 2;
  estado = 1;
  sendInfo(procesoActual,pasoProceso,estado);
  
//LECTURA DE VARIABLES            
  float temperaturaCoccion = tempCoc;                         //Variable con la temperatura del proceso
  float tiempoCoccion = tiempoCoc;                            //Variable del tiempo del proceso en minutos

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
//Envio mensaje de fin de proceso.
  if (falloProceso) estado = 3;
  else estado = 2;
  recovery = 0;
  sendInfo(procesoActual,pasoProceso,estado);
  finProceso(procesoActual,falloProceso);
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
//Confirmacion del inicio de proceso de trasvase
  Serial.println("O3");
  procesoActual = 3;
  estado = 1;
  sendInfo(procesoActual,0,estado);
  
  
//Trasvase ON
  digitalWrite(bombaFrio,HIGH);
  delay(retrasoBombas);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(peltier,HIGH);

//Control de tiempo y sensor de liquido
    Serial.println("------------------------");
    Serial.print("El tiempo de seguridad es de: ");
    Serial.print("10");
    Serial.println(" Minutos");
    Serial.println("------------------------");
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (10 * 60);
    long tiempoCancelacion = tiempoActual + 5;
    do{
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      if (tiempoActual >= tiempoCancelacion){
        tiempoCancelacion = tiempoActual + 5;
        comprobarCancelar();
        if (falloProceso){
          break;
        }
      }
      if (tiempoRestante <= 0 || sensorLiquido == HIGH) break;
    delay(1000);
  }while(true);
//Trasvase OFF  
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
  
//Envio mensaje de fin de proceso.
  if (falloProceso) estado = 3;
  else estado = 2;
  sendInfo(procesoActual,0,estado);
  finProceso(procesoActual,falloProceso);
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
void fermentacion(byte pasoProceso){
//Confirmacion para RASPBERRY del inicio de proceso de fermentacion
  Serial.println("O4");
  procesoActual = 4;
  estado = 1;
  sendInfo(procesoActual,pasoProceso,estado);
  
//LECTURA DE VARIABLES
  float temperaturaFermentacion = tempFermen;
  int tiempoFermentacion = tiempoFermen;
  gettime();
  tiempoi = tiempoActual;
  tiempof = tiempoi + (tiempoFermen * 2629750);
  Serial.println(tiempof);
  long tiempoCancelacion = tiempoActual + 5;
  long tiempoMtiempo = tiempoActual;
  do{
    gettime();
    tiempoRestante = tiempof - tiempoActual;
    if (tiempoActual >= tiempoCancelacion){
      tiempoCancelacion = tiempoActual + 5;
      comprobarCancelar();
      if (falloProceso){
        break;
      }
    }
    if (tiempoActual >= tiempoMtiempo){
      tiempoMtiempo = tiempoActual + 60;
      
    }
    if (tiempoRestante <= 0) break;
      delay(1000);
  }while(true);
//PUESTA A CERO FINAL
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  
//Envio mensaje de fin de proceso.
  if (falloProceso) estado = 3;
  else {estado = 2; c_nokia_c();}
  recovery = 0;
  sendInfo(procesoActual,pasoProceso,estado);
  finProceso(procesoActual,falloProceso);
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
void calentar( int temperaturaProceso, long tiempoProceso){
//TRATAMIENTO DE LAS VARIABLES
  //Tratamiento de la ventana de temperatura
    Serial.println("------------------------");
    Serial.print("El proceso dura: ");
    Serial.print(tiempoProceso);
    Serial.println(" Minutos");
    Serial.println("------------------------");
    int tmax = temperaturaProceso+anchoVentana;
    int tmin = temperaturaProceso-anchoVentana;
    
    gettime();
    tiempoi = tiempoActual;
    tiempof = tiempoi + (tiempoProceso * 60);
    int tiempoProcesoSeg = tiempof - tiempoi;
    long tiempoCancelacion = tiempoActual + 5;
    int tiempoPorcentaje = tiempoActual + 2;
    
    do{
      if (tiempoActual >= tiempoCancelacion){
        tiempoCancelacion = tiempoActual + 5;
        comprobarCancelar();
        if (falloProceso){
          break;
        }
      }
      gettime();
      tiempoRestante = tiempof - tiempoActual;
      if (tiempoRestante <= 0) {break; porcentaje = 100;}
      if (tiempoActual >= tiempoPorcentaje){
        tiempoPorcentaje = tiempoActual + 2;
        int timepoIncremental = tiempoProcesoSeg - tiempoRestante;
        porcentaje = (timepoIncremental * 100) / tiempoProcesoSeg;
        Serial.print("Leeva el ");
        Serial.print(porcentaje);
        Serial.print("%");
        Serial.println(" completado");
      }
      
  //Tratamiento de la temperatura
    int sensorTemperatura = analogRead(pinSonda);
    float milivoltios = (sensorTemperatura / 1023.0) * 3300;
    float celsius = milivoltios / 10;
  //Mantenimiento de la ventana de temperatura
    if(celsius > tmax){digitalWrite(resis,LOW);}
    if(celsius < tmin){digitalWrite(resis,HIGH);}
    delay(1000);
  }while(true);
  
//PUESTA A CERO FINAL
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
}

void comprobarCancelar() {
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Comprobación de cancelación");
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/menu.php?fallo=1&IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client,consulta);  // Request destination.
    int httpCode = http.GET(); // Send the request.
      if (httpCode == 200 || httpCode == 201) {
        String stringcancelar = http.getString();
        int cancelar = stringcancelar.toInt();
        if (cancelar == 1){
          falloProceso = 1;
          http.end();
        }
    }else{
      Serial.println("El servidor no responde");
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

void leerReceta(){
  if (WiFi.status() == WL_CONNECTED) {
    
    String peticion = "https://192.168.1.150/arduino/pedirdatos.php?id=";
    peticion = peticion + IDreceta;
    Serial.println("------------------------------");
    Serial.print("Petición al servidor: ");
    Serial.println(peticion);
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    http.begin(*client,peticion);  // Request destination.
    int httpCode = http.GET(); // Send the request.
    Serial.println(httpCode);
    if (httpCode == 200 || httpCode == 201) { //Check the returning code
      String datos = http.getString(); 
      http.end();   //Close connection
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
      //tempMacer = stempMacer.toFloat();
      int numParametros = count(stempMacer);
      for (int i = 0;i < numParametros;i ++){
        tempMacer[i] = s.separa(stempMacer, ':', i);
      }
      
      

    //Procesar datos tiempo la Maceración
      int ptiempoMacer = datos.indexOf("tiempoMacer=");
      String stiempoMacer = "";
      for (int i = ptiempoMacer + 12; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stiempoMacer += datos[i];
      }
      //tiempoMacer = stiempoMacer.toInt();
      numParametros = count(stiempoMacer);
      for (int i = 0;i < numParametros;i ++){
        tiempoMacer[i] = s.separa(stiempoMacer, ':', i);
      }
      

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
      tiempoFermen = stiempoFermen.toInt();
      

    //Mostrar información de la receta por Serial
  if (tempMacer != 0){
      //Nombre de la cerveza
        Serial.print("Nombre de la cerveza= ");
        Serial.println(nombre);
      //Temperaturas
        Serial.print("Temperatura del proceso Maceración= ");
        //Serial.println(tempMacer);
        Serial.println(stempMacer);
        Serial.print("Temperatura del proceso Cocción= ");
        Serial.println(tempCoc);
        Serial.print("Temperatura del proceso de Fermentación= ");
        Serial.println(tempFermen);
      //Tiempos en segundos
        Serial.print("Tiempo en Minutos del proceso Maceración= ");
        Serial.println(stiempoMacer);
        Serial.print("Tiempo en Segundos del proceso Cocción= ");
        Serial.println(tiempoCoc);
        Serial.print("Tiempo en Meses del proceso Fermentación= ");
        Serial.println(tiempoFermen);
    }else{
      Serial.println("La receta no existe");
    }
    }else{

      Serial.println("El servidor no responde");

    }

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
void finProceso (unsigned char proceso,bool error){
//Variables locales
  String mensaje = "Proceso ";
//Conversion a String
  mensaje.concat(proceso);
  mensaje.concat(" Fallo ");
  mensaje.concat(error);

  if (falloProceso){
    if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String consulta = "https://192.168.1.150/arduino/menu.php?resetfallo=1&IDplaca=";
    consulta = consulta + IDplaca;
    http.begin(*client, consulta);
    http.GET();
    http.end();
    falloProceso = 0;
  }
  }
  
  
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}

void sendInfo(unsigned char proceso,byte pasoProceso,unsigned char estado) {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    //client->setFingerprint(fingerprint);
    client->setInsecure();
    String peticion = "https://192.168.1.150/arduino/info.php?proceso=";
    peticion = peticion + proceso;
    peticion = peticion + "&estado=";
    peticion = peticion + estado;
    peticion = peticion + "&pasoProceso=";
    peticion = peticion + pasoProceso;
    peticion = peticion + "&IDplaca=";
    peticion = peticion + IDplaca;
    peticion = peticion + "&porcentaje=";
    peticion = peticion + porcentaje;
    peticion = peticion + "&receta=";
    peticion = peticion + IDreceta;
    http.begin(*client, peticion);
    http.GET();
    http.end();
  }
}


int count(String str){
  int a = 1;
  for (int i = 0; i < str.length(); i ++){
      if (str[i] == ':') {a = a + 1;}
  }
  return a;
}

/*
 * Funcion para reproducir la cancion.
 * Uso de la funcion tone:
 * tone(pin, frequency, duration)
 * 
 * Parametros: pin del zumbador (Poner "zumbador" para llamar al pin 3)
 * No devuelve nada
 */

void cancion2(){
delay(100);
  tone(zumbador,440,100);
  tone(zumbador,587.33,100);
  delay(100);  
  delay(100);
  tone(zumbador,293.66,200);
}

void c_nokia_c(){
    tone(zumbador, 1318, 124.99996875);
    delay(124.99996875);
    delay(5.43478125);
    tone(zumbador, 1174, 130.43475);
    delay(130.43475);
    delay(10.8695625);
    tone(zumbador, 739, 244.56515625);
    delay(244.56515625);
    delay(10.8695625);
    tone(zumbador, 830, 244.56515625);
    delay(244.56515625);
    delay(5.43478125);
    tone(zumbador, 1108, 114.13040625);
    delay(114.13040625);
    tone(zumbador, 987, 141.3043125);
    delay(141.3043125);
    tone(zumbador, 587, 277.17384375);
    delay(277.17384375);
    tone(zumbador, 659, 298.91296875);
    delay(298.91296875);
    tone(zumbador, 987, 135.86953125);
    delay(135.86953125);
    delay(5.43478125);
    tone(zumbador, 880, 168.47821875);
    delay(168.47821875);
    tone(zumbador, 554, 315.2173125);
    delay(315.2173125);
    delay(5.43478125);
    
    tone(zumbador, 659, 70.65215625);
    delay(70.65215625);
    tone(zumbador, 880, 913.04325);
    delay(913.04325);
  }
void cancion1(){
  
  delay(100);
  tone(zumbador,659.26);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,783.99);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,880);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,880);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,880);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,987.77);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,1046.77);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,1046.77);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,1046.77);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,1174.66);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,987.77);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,987.77);
  delay(900);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,880);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,783.99);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,783.99);
  delay(150);
  noTone(zumbador);
  delay(100);
  
  tone(zumbador,880);
  delay(150);
  noTone(zumbador);
  delay(100);
  
}
