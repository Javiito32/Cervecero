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
  #include <ArduinoJson.h>                    // Para los datos JSON
  #include <ESP8266WiFi.h>                    // Para el modulo ESP8266
  #include <ESP8266HTTPClient.h>              // ESP como cliente
  #include <WiFiClientSecureBearSSL.h>        // ESP como cliente seguro https
  #include <Wire.h>                           // Para interfaz I2C para comunicaciones de dispositivos por direcciones
  #include <RTClib.h>                         // Para el manejo del modulo RTC
  #include <TimeLib.h>                        // Libreria para gestionar las conversiones de tiempo
  #include <Separador.h>                      // Como su propio nombre indica separa cadenas de datos
  #include <RunningMedian.h>                  // Hace una lectura precisa de los sensores
  #include <WiFiManager.h>                    // Interfaz para conectar el modulo a una red WiFi
  #include <DNSServer.h>                      // Va con la libreria de arriba
  #include <DoubleResetDetector.h>            // Detecta cuando se ha reiniciado el modulo 2 veces en un periodo de tiempo especificado
  #include <ESP8266httpUpdate.h>              // Para las actualizaciones de firmware
  #include <PubSubClient.h>                   // Para las comunicaciones MQTT
  #include <LiquidCrystal_I2C.h>
  
  
//LAYOUT Pines
  #define pinSonda A0                    // Sonda de la temperatura
  #define resis D0                       // Resistencia para calentar               
  #define bombaRecirculacion D3          // Bomba de recirculacion 230V
  #define bombaTrasvase D4               // Bomba trasvase 230V
  #define bombaFrio D5                   // Bomba refrigeracion 230V
  #define peltier D6                     // Celulas Peltier
  #define sensorLiquido D7               // Sensor de liquido en tubo
  #define zumbador D8                    // Zumbador para reproducir canciones
  #define DRD_TIMEOUT 2                  // El tiempo en segundos que va a esperar para el doble reset "esta hecho con un delay y esto no se utiliza"
  #define DRD_ADDRESS 0                  // RTC Memory Address for the DoubleResetDetector to use

  
//Variables configurables
  const float anchoVentana = 1;               // Rango para la temperatura
  const float tiempoTrasvase = 210000;        // Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;             // Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables globales
  int dato;                                   // Dato leido para entrar el menu
  unsigned long tiempoi;                      // Tiempo inicial para los procesos en seg
  unsigned long tiempof;                      // Tiempo final para los procesos en seg
  unsigned long tiempoActual;                 // Tiempo actual del proceso en seg
  long tiempoRestante;                        // Tiempo que falta para el final de los procesos en seg
  String tempMacer[10];                       // Temperatura de maceración de la receta seleccionada
  String tiempoMacer[10];                     // Tiempo maceración de la recta selecionada
  String tempCoc[10];                         // Temperatura de cocción de la receta seleccionada
  String tiempoCoc[10];                       // Tiempo cocción de la recta selecionada
  unsigned long tiempoTrans;                  // Tiempo transvase de la recta selecionada
  String tempFermen[10];                      // Temperatura de fermentación de la receta seleccionada
  String tiempoFermen[10];                    // Tiempo fermentación de la recta selecionada
  bool falloProceso = 0;                      // Guarda si falla el tiempo
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
  String currentVersion = "1.0.6";
  String host = "https://192.168.1.150/php/arduino/";
  String updatesServer = "192.168.1.150";
  
  //const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
    
//Objetos
  HTTPClient http;                                    // Objeto para la clase HTTPClient.
  DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);  // Objeto para la clase DoubleResetDetector.
  RTC_DS3231 rtc;                                     // Objeto para la clase RTC_DS3231.
  Separador s;                                        // Objeto para la clase Separador.
  WiFiManager wifiManager;                            // Objeto para la clase WiFiManager.
  LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/*
 * CICLO DE ARRANQUE
 * Configuramos los pines. Puesta a cero inicial.
 */
void setup(){
  
//Inicializamos las cosas
  Serial.begin(115200);
  WiFi.begin();
  Wire.begin(D2,D1);
  lcd.begin();
  delay(10);
  
//Configuracion de pines
  //pinMode(2, OUTPUT);
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

//Detectamos si se ha pulsado el reset 2 veces para entrar en la configuracion del WiFi
if (drd.detectDoubleReset()) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("----  Modo  ----");
  lcd.setCursor(0,1);
  lcd.print(" Configuracion");
  //Serial.println("Double Reset Detected");
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
  //digitalWrite(2, HIGH);
  Serial.println("");
  Serial.print("Connecting");
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Iniciando...");
  

  while (WiFi.status() != WL_CONNECTED) {       //Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  //digitalWrite(2, LOW);

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());               //Mostrar la IP que tiene el dispositivo
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  mac = WiFi.macAddress();

  Serial.println("++++++++++++++++++++++++++++++++");
  Serial.println(         "Cervecero 2.0");
  Serial.println(         "Version: " + currentVersion);
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
  inicio();
}
