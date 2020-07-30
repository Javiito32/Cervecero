/*
 *  Notas de la version 1.0.6:
 *  - Actualizacion de pines para el NodeMCU.
 *  - Sustitución de las funciones que controlaban el tiempo de los procesos por 
 *    el tiempo con un modulo a tiempo real RTC.
 *  - Se han añadido las librerias pertinentes para el correcto funcionameinto.
 *  - En la ecuacion de temperatura hemos cambiado el valor del voltaje de referencia
 *    de 5000mV (Arduino) a 3300mV (NodeMCU).
 */

/*
 * PINES Y VARIABLES
 */                    
  #include <ArduinoJson.h>                              // Para los datos JSON
  #include <ESP8266WiFi.h>                              // Para el modulo ESP8266
  #include <ESP8266HTTPClient.h>                        // ESP como cliente
  #include <WiFiClientSecureBearSSL.h>                  // ESP como cliente seguro https
  #include <Wire.h>                                     // Para interfaz I2C para, comunicaciones de dispositivos por direcciones
  #include <RTClib.h>                                   // Para el manejo del modulo RTC
  #include <TimeLib.h>                                  // Libreria para gestionar las conversiones de tiempo
  #include <Separador.h>                                // Como su propio nombre indica separa cadenas de datos
  #include <RunningMedian.h>                            // Hace una lectura precisa de los sensores
  #include <WiFiManager.h>                              // Interfaz para conectar el modulo a una red WiFi
  #include <DNSServer.h>                                // Va con la libreria de arriba
  #include <DoubleResetDetector.h>                      // Detecta cuando se ha reiniciado el modulo 2 veces en un periodo de tiempo especificado
  #include <ESP8266httpUpdate.h>                        // Para las actualizaciones de firmware
  #include <PubSubClient.h>                             // Para las comunicaciones MQTT
  #include <LiquidCrystal_I2C.h>                        // Para el control de la pantalla LCD
  
  
//LAYOUT Pines
  #define pinSonda A0                                   // Sonda de la temperatura
  #define resis D0                                      // Resistencia para calentar               
  #define bombaRecirculacion D3                         // Bomba de recirculacion 230V
  #define bombaTrasvase D4                              // Bomba trasvase 230V
  #define bombaFrio D5                                  // Bomba refrigeracion 230V
  #define peltier D6                                    // Celulas Peltier
  #define sensorLiquido D7                              // Sensor de liquido en tubo
  #define zumbador D8                                   // Zumbador para reproducir canciones
  #define DRD_TIMEOUT 2                                 // El tiempo en segundos que va a esperar para el doble reset "esta hecho con un delay y esto no se utiliza"
  #define DRD_ADDRESS 0                                 // RTC Memory Address for the DoubleResetDetector to use

  
//Variables configurables
  const float rangoTemp = 1;                            // Rango para la temperatura
  const float tiempoTrasvase = 210000;                  // Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;                       // Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables globales
  int dato;                                             // Dato leido para entrar el menu
  unsigned long tiempoi;                                // Tiempo inicial para los procesos en seg
  unsigned long tiempof;                                // Tiempo final para los procesos en seg
  unsigned long tiempoActual;                           // Tiempo actual del proceso en seg
  long tiempoRestante;                                  // Tiempo que falta para el final de los procesos en seg
  String tempMacer[10];                                 // Temperatura de maceración de la receta seleccionada
  String tiempoMacer[10];                               // Tiempo maceración de la recta selecionada
  String tempCoc[10];                                   // Temperatura de cocción de la receta seleccionada
  String tiempoCoc[10];                                 // Tiempo cocción de la recta selecionada
  unsigned long tiempoTrans;                            // Tiempo transvase de la recta selecionada
  String tempFermen[10];                                // Temperatura de fermentación de la receta seleccionada
  String tiempoFermen[10];                              // Tiempo fermentación de la recta selecionada
  bool falloProceso = 0;                                // Guarda si falla el tiempo
  int procesoActual;                                    // El proceso que se esta ejecutando
  int pasoProceso;                                      // El paso del proceso que se esta ejecutando
  int estado;                                           // 1 - Iniciado, 2 - Finalizado, 3 - Cancelado
  String mac;                                           // La direccion MAC de el WiFi
  int IDplaca;                                          // Identificador unico de la placa, es un número
  int IDreceta = 0;                                     // El ID de la receta de la BDD
  byte porcentaje;                                      // Porcentaje completado del proceso
  bool recovery;                                        // 0 - Nada que recuperar, 1 - Recupera el proceso que estubiera haciendo
  int recoveryTiempoRestante;                           // Variable de recovery
  int recoveryProceso;                                  // Variable de recovery
  int recoveryPasoProceso;                              // Variable de recovery
  int tiempoProcesoSeg;                                 // El tiempo del proceso en segundos
  String currentVersion = "1.0.7";                      // Versión del Firmware
  String host = "https://192.168.1.150/php/arduino/";   // Servidor de PHP donde manda y recibe información
  String updatesServer = "192.168.1.150";               // Servidor de actualizaciones
  
  //const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
    
//Objetos
  HTTPClient http;                                      // Objeto para la clase HTTPClient.
  DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);    // Objeto para la clase DoubleResetDetector.
  RTC_DS3231 rtc;                                       // Objeto para la clase RTC_DS3231.
  Separador s;                                          // Objeto para la clase Separador.
  WiFiManager wifiManager;                              // Objeto para la clase WiFiManager.
  LiquidCrystal_I2C lcd(0x27,16,2);                     // Objeto para la clase lcd, establecer el tipo de lcd que tenemos: en este caso una de 16x2 y la address (dirección) 0x27

void setup(){
  
//Inicializamos las cosas
  Serial.begin(115200);                                 // Iniciamos el serial
  WiFi.begin();                                         // Iniciamos WiFi
  Wire.begin(D2,D1);                                    // Iniciamos las conexiones Wire
  lcd.begin();                                          // Iniciamos la lcd
  delay(10);
  beep(1);                                              // Hacemos un pitido
  
//Configuracion de pines
  pinMode(resis,OUTPUT);
  pinMode(bombaRecirculacion,OUTPUT);
  pinMode(bombaTrasvase,OUTPUT);
  pinMode(bombaFrio,OUTPUT);
  pinMode(peltier,OUTPUT);
  pinMode(sensorLiquido,INPUT);
  pinMode(zumbador,OUTPUT);

//Seteamos pines a LOW
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);

//Detectamos si se ha pulsado el reset mientras el inicio para entrar en la configuracion del WiFi
if (drd.detectDoubleReset()) {
  beep(2);                                              // Hacemos 2 pitidos
  lcd.clear();                                          // Limpiamos lo que hubiese escrito en la lcd
  lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
  lcd.print("----  Modo  ----");
  lcd.setCursor(0,1);                                   // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
  lcd.print(" Configuracion");
  wifiManager.setConfigPortalTimeout(180);              // Si en 2 minutos no se ha conectado ningún dispositivo para configurar el wifi, se cierra
  wifiManager.startConfigPortal("Cervecero_2.0");       // Se inicia el portal cautivo para la configuración
  } /*else {
    Serial.println("No Double Reset Detected");
  }*/
  
// Conectar con la red WiFi
/*do{
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Cervecero_2.0");
}while (WiFi.status() != WL_CONNECTED);*/
  Serial.println("");
  Serial.print("Connecting");
  
  lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
  lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
  lcd.print("Conectando WiFi");                            
  

  while (WiFi.status() != WL_CONNECTED) {               // Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
  }
  drd.stop();                                           // Dejamos de detectar el reset de la placa para entrar en el modo configuracion ya que si llegamos a este punto significa que ya tenemos WiFi
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());                       // Mostrar la IP que tiene el dispositivo
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());                    // Mostramos la mac del dispositivo
  mac = WiFi.macAddress();

  Serial.println("++++++++++++++++++++++++++++++++");
  Serial.println(         "Cervecero 2.0");
  Serial.println(         "Version: " + currentVersion);
  Serial.println("++++++++++++++++++++++++++++++++");
  lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
  lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
  lcd.print("Iniciando...");
  /*                          
   * Obtenemos el ID de placa asociado a la MAC
   */
  while (true){
    String datos = peticion("get_id.php","mac=" + mac);
      if (datos != "fallo") {
        //Serial.println(stringIDplaca);
        IDplaca = datos.toInt();
        Serial.println("------------------------------");
        Serial.print("El ID de la placa es el: ");
        Serial.println(IDplaca);
        Serial.println("------------------------------");
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No se pudo obtener el ID de placa");
        Serial.println("------------------------------");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el ID de placa");
        delay(10);
      }
  }

  checkrecovery();                                      // Comprobamos si hay procesos pendientes
  if (recovery != 1) { checkforUpdates();}              // Comprobamos si hay actualizaciones y si el usuario quiere actualizar
  
  if (recovery == 1){                                   // Si hay procesos pendientes hara lo siguiente
    leerReceta();                                       // Leer la receta
    pasoProceso = recoveryPasoProceso;
    recoveryProcesos(recoveryProceso);                  // Esto arranca el proceso que haya que no se terminó en caso de que lo hubiese
  }
}
  



void loop(){
  lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
  lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
  lcd.print("Cervecero v" + currentVersion);
  lcd.setCursor(0,1);                                   // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
  lcd.print(" Ready");
  Serial.println("------------------------------");
  Serial.println("Ready");
  Serial.println("------------------------------");

 /* 
  * Menu de consultas con PHP MySQL en formato JSON 
  */
  json_menu();
 /*
  * Menu de consultas PHP con MySQL
  */
  //SQL_menu();
  //menuinicio(dato);
}
