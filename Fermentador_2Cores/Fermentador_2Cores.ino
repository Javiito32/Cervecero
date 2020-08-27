
/*
 *  Notas de la version 1.0.6:
 *  - Actualizacion de pines para el NodeMCU.
 *  - Sustitución de las funciones que controlaban el tiempo de los procesos por 
 *    el tiempo con un modulo a tiempo real RTC.
 *  - Se han añadido las librerias pertinentes para el correcto funcionameinto.
 *  - En la ecuacion de temperatura hemos cambiado el valor del voltaje de referencia
 *    de 5000mV (Arduino) a 3300mV (NodeMCU).
 */

 
  #include "config.h"                                   // Archivo de configuración
  #define DOUBLERESETDETECTOR_DEBUG       false
  #define ESP_DRD_USE_EEPROM      true

  TaskHandle_t Task1 = NULL;

/*
 * PINES Y VARIABLES
 */   
               
  #include <ArduinoJson.h>                              // Para los datos JSON
  //#include <ESP8266WiFi.h>                              // Para el modulo ESP8266
  #include <WiFi.h>
  //#include <ESP8266HTTPClient.h>                        // ESP como cliente
  #include <HTTPClient.h>
  //#include <WiFiClientSecureBearSSL.h>                  // ESP como cliente seguro https
  //#include <WiFiClientSecure.h>
  #include <Wire.h>                                     // Para interfaz I2C para, comunicaciones de dispositivos por direcciones
  #include <RTClib.h>                                   // Para el manejo del modulo RTC
  #include <TimeLib.h>                                  // Libreria para gestionar las conversiones de tiempo
  #include <Separador.h>                                // Como su propio nombre indica separa cadenas de datos
  #include <RunningMedian.h>                            // Hace una lectura precisa de los sensores
  #include <WiFiManager.h>                              // Interfaz para conectar el modulo a una red WiFi
  #include <DNSServer.h>                                // Va con la libreria de arriba
  #include <ESP_DoubleResetDetector.h>                  // Detecta cuando se ha reiniciado el modulo 2 veces en un periodo de tiempo especificado
  //#include <ESP8266httpUpdate.h>                        // Para las actualizaciones de firmware
  #include <HTTPUpdate.h>
  //#include <PubSubClient.h>                             // Para las comunicaciones MQTT
  #ifdef pantallaLCD
    #include <LiquidCrystal_I2C.h>                        // Para el control de la pantalla LCD
  #endif
  
  
//LAYOUT Pines
  #define LED_BUILTIN       25
  #define pinSonda 13                                   // Sonda de la temperatura
  #define resis 12                                      // Resistencia para calentar               
  #define bombaRecirculacion 14                         // Bomba de recirculacion 230V
  #define bombaTrasvase 27                              // Bomba trasvase 230V
  #define bombaFrio 33                                  // Bomba refrigeracion 230V
  #define peltier 32                                    // Celulas Peltier
  #define sensorLiquido 35                              // Sensor de liquido en tubo
  #define zumbador 34                                   // Zumbador para reproducir canciones

  
  #define DRD_TIMEOUT 10                                // El tiempo en segundos que va a esperar para el doble reset
  #define DRD_ADDRESS 0                                 // RTC Memory Address for the DoubleResetDetector to use

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
  byte procesoActual;                                    // El proceso que se esta ejecutando
  byte faseProceso;                                      // El paso del proceso que se esta ejecutando
  byte estado;                                           // 1 - Iniciado, 2 - Finalizado, 3 - Cancelado
  String mac;                                           // La direccion MAC de el WiFi
  int IDplaca;                                          // Identificador unico de la placa, es un número
  int IDreceta = 0;                                     // El ID de la receta de la BDD
  byte porcentaje;                                      // Porcentaje completado del proceso
  bool recovery;                                        // 0 - Nada que recuperar, 1 - Recupera el proceso que estubiera haciendo
  int recoveryTiempoRestante;                           // Variable de recovery
  byte recoveryProceso;                                  // Variable de recovery
  byte recoveryPasoProceso;                              // Variable de recovery
  int tiempoProcesoSeg;                                 // El tiempo del proceso en segundos
  String currentVersion = "1.0.7";                      // Versión del Firmware
  String host = "http://192.168.1.150/php/arduino/";   // Servidor de PHP donde manda y recibe información
  String updatesServer = "192.168.1.150";               // Servidor de actualizaciones
    
//Objetos
  HTTPClient http;                                      // Objeto para la clase HTTPClient.
  DoubleResetDetector* drd;                             // Objeto para la clase DoubleResetDetector.
  RTC_DS3231 rtc;                                       // Objeto para la clase RTC_DS3231.
  Separador s;                                          // Objeto para la clase Separador.
  WiFiManager wifiManager;                              // Objeto para la clase WiFiManager.
  #ifdef pantallaLCD
    LiquidCrystal_I2C lcd(0x27,16,2);                     // Objeto para la clase lcd, establecer el tipo de lcd que tenemos: en este caso una de 16x2 y la address (dirección) 0x27
  #endif
void setup(){

  xTaskCreatePinnedToCore(
    uploadToLOG,      // Function that should be called
    "LOG",            // Name of the task (for debugging)
    1000,               // Stack size (bytes)
    NULL,               // Parameter to pass
    0,                  // Task priority
    &Task1,               // Task handle
    1);          // Core you want to run the task on (0 or 1)
  
//Inicializamos las cosas
  Serial.begin(115200);                                 // Iniciamos el serial
  WiFi.begin();                                         // Iniciamos WiFi
  Wire.begin(15, 4);                                    // Iniciamos las conexiones Wire
  #ifdef pantallaLCD
    lcd.begin();                                          // Iniciamos la lcd
  #endif
  delay(10);
  
//Configuracion de pines
  pinMode(LED_BUILTIN, OUTPUT);
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

drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
if (drd->detectDoubleReset()) {
  digitalWrite(LED_BUILTIN, HIGH);
  #ifdef pantallaLCD
    lcd.clear();                                          // Limpiamos lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    lcd.print("----  Modo  ----");
    lcd.setCursor(0,1);                                   // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
    lcd.print(" Configuracion");
  #endif
  wifiManager.setConfigPortalTimeout(180);              // Si en 2 minutos no se ha conectado ningún dispositivo para configurar el wifi, se cierra
  wifiManager.startConfigPortal("Cervecero_2.0");       // Se inicia el portal cautivo para la configuración
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    #ifdef debug
      Serial.println("No Double Reset Detected");
    #endif
  }
  delay(2000);
  drd->stop();
  
// Conectar con la red WiFi
  #ifdef debug
    Serial.println("");
    Serial.print("Connecting");
  #endif

  #ifdef pantallaLCD
    lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    lcd.print("Conectando WiFi");                            
  #endif
  

  while (WiFi.status() != WL_CONNECTED) {               // Mostrar ... mientras se conacta al WiFi
    delay(500);
    #ifdef debug
      Serial.print(".");
    #endif
  }
  mac = WiFi.macAddress();
  #ifdef debug
    Serial.println("");
    Serial.println("WiFi connected");

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());                       // Mostrar la IP que tiene el dispositivo
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());                    // Mostramos la mac del dispositivo

    Serial.println("++++++++++++++++++++++++++++++++");
    Serial.println(         "Cervecero 2.0");
    Serial.println(         "Version: " + currentVersion);
    Serial.println("++++++++++++++++++++++++++++++++");
  #endif
  #ifdef pantallaLCD
    lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    lcd.print("Iniciando...");
  #endif
  
  /*                          
   * Obtenemos el ID de placa asociado a la MAC
   */
  while (true){
    String datos = peticion("get_id.php","mac=" + mac);
      if (datos != "fallo") {
        IDplaca = datos.toInt();
        #ifdef debug
          Serial.println("------------------------------");
          Serial.print("El ID de la placa es el: ");
          Serial.println(IDplaca);
          Serial.println("------------------------------");
        #endif
        break;
      }else{
        #ifdef debug
        Serial.println("------------------------------");
        Serial.println("No se pudo obtener el ID de placa o no está registrada");
        Serial.println("------------------------------");
        #endif
        #ifdef pantallaLCD
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error al obtener");
        lcd.setCursor(0,1);
        lcd.print("el ID de placa");
        #endif
        delay(1000);
      }
  }

  checkrecovery();                                      // Comprobamos si hay procesos pendientes
  if (!recovery){                                       // Comprobamos si hay actualizaciones y si el usuario quiere actualizar
    checkforUpdates();
  } else{                                               // Si hay procesos pendientes hara lo siguiente
    leerReceta();                                       // Leer la receta
    faseProceso = recoveryPasoProceso;
    recoveryProcesos(recoveryProceso);                  // Esto arranca el proceso que haya que no se terminó en caso de que lo hubiese
  }
}
  



void loop(){
  #ifdef pantallaLCD
    lcd.clear();                                          // Limpia lo que hubiese escrito en la lcd
    lcd.setCursor(0,0);                                   // Ponemos el cursor para empezar a escrivir en la linea 1 celda 0
    lcd.print("Cervecero v" + currentVersion);
    lcd.setCursor(0,1);                                   // Ponemos el cursor para empezar a escrivir en la linea 2 celda 0
    lcd.print(" Ready");
  #endif
  #ifdef debug
    Serial.println("------------------------------");
    Serial.println("Ready");
    Serial.println("------------------------------");
  #endif

 /* 
  * Menu de consultas con PHP MySQL en formato JSON 
  */
  #ifdef new_menu
    json_menu();
  #endif
 /*
  * Menu de consultas PHP con MySQL
  */
  #ifdef old_menu
    SQL_menu();
    menuinicio(dato);
  #endif
}
