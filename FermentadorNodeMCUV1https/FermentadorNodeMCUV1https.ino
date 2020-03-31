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
  #include <RunningMedian.h>
  #include <DNSServer.h>
  #include <WiFiManager.h>
  #include <DoubleResetDetector.h>
  #include <ESP8266httpUpdate.h>
  
  
//LAYOUT Pines
  #define pinSonda A0                    //Sonda de la temperatura
  #define resis D0                       //Resistencia para calentar               
  #define bombaRecirculacion D3          //Bomba de recirculacion 230V
  #define bombaTrasvase D4               //Bomba trasvase 230V
  #define bombaFrio D5                   //Bomba refrigeracion 230V
  #define peltier D6                     //Celulas Peltier
  #define sensorLiquido D7               //Sensor de liquido en tubo
  #define zumbador D8                    //Zumbador para reproducir canciones
  #define DRD_TIMEOUT 2
  #define DRD_ADDRESS 0                  // RTC Memory Address for the DoubleResetDetector to use


  
//Variables configurables
  const float anchoVentana = 1;               //Rango para la temperatura
  const float tiempoTrasvase = 210000;        //Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;             //Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables globales
  //const char* ssid = "";                      //Nombre de la red WiFi a la que se va a conectar
  //const char* password = "";                  //Contraseña de la red WiFi a la que se va a conectar
  int dato;                                   //Dato leido para entrar el menu
  unsigned long tiempoi;                      //Tiempo inicial para los procesos en seg
  unsigned long tiempof;                      //Tiempo final para los procesos en seg
  unsigned long tiempoActual;                 //Tiempo actual del proceso en seg
  long tiempoRestante;                        //Tiempo que falta para el final de los procesos en seg
  String tempMacer[10];                       //Temperatura de maceración de la receta seleccionada
  String tiempoMacer[10];                     //Tiempo maceración de la recta selecionada
  String tempCoc[10];                         //Temperatura de cocción de la receta seleccionada
  String tiempoCoc[10];                       //Tiempo cocción de la recta selecionada
  unsigned long tiempoTrans;                  //Tiempo transvase de la recta selecionada
  String tempFermen[10];                      //Temperatura de fermentación de la receta seleccionada
  String tiempoFermen[10];                    //Tiempo fermentación de la recta selecionada
  bool falloProceso = 0;                      //Guarda si falla el tiempo
  int procesoActual;
  int pasoProceso;
  int estado;
  String mac;
  int IDplaca;
  int IDreceta;
  byte porcentaje;
  bool recovery;
  int recoveryTiempoRestante;
  int recoveryProceso;
  int recoveryPasoProceso;
  int tiempoProcesoSeg;
  String currentVersion = "1.0.4";
  
  //const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
    
//Objetos
  HTTPClient http;                              // Object of the class HTTPClient.
  DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
  RTC_DS3231 rtc;                               // crea objeto del tipo RTC_DS3231
  Separador s;                                  //Objeto para separar datos
  WiFiManager wifiManager;

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
  pinMode(2, OUTPUT);
  pinMode(resis,OUTPUT);
  pinMode(bombaRecirculacion,OUTPUT);
  pinMode(bombaTrasvase,OUTPUT);
  pinMode(bombaFrio,OUTPUT);
  pinMode(peltier,OUTPUT);
  pinMode(sensorLiquido,INPUT);
  pinMode(zumbador,OUTPUT);

  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);


if (drd.detectDoubleReset()) {
    //Serial.println("Double Reset Detected");
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    wifiManager.setConfigPortalTimeout(180);
    wifiManager.startConfigPortal("Cervecero_2.0");
  } /*else {
    Serial.println("No Double Reset Detected");
  }*/
  delay(2000);
  drd.stop();
  
// Conectar con la red WiFi
/*do{
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cervecero_2.0");
}while (WiFi.status() != WL_CONNECTED);*/
  digitalWrite(2, HIGH);
  Serial.println("");
  Serial.print("Connecting");
  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED) {       //Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(2, LOW);

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());               //Mostrar la IP que tiene el dispositivo
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  mac = WiFi.macAddress();

  Serial.println("++++++++++++++++++++++++++++++++");
  Serial.println(         "Cervecero 2.0");
  Serial.println(         "Version:" + currentVersion);
  Serial.println("++++++++++++++++++++++++++++++++");
  
  getID();
  checkrecovery();
  checkforUpdates();
  startUpdate();
  
  
  if (recovery == 1){
    leerReceta();
    pasoProceso = recoveryPasoProceso;
    recoveryProcesos(recoveryProceso);
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
