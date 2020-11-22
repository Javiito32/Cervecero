
/*
 *  Notas de la version 1.0.7:
 *  - Actualizacion de pines para el NodeMCU.
 *  - Sustitución de las funciones que controlaban el tiempo de los procesos por 
 *    el tiempo con un modulo a tiempo real RTC.
 *  - Se han añadido las librerias pertinentes para el correcto funcionameinto.
 *  - En la ecuacion de temperatura hemos cambiado el valor del voltaje de referencia
 *    de 5000mV (Arduino) a 3300mV (NodeMCU).
 */

 
#include "config.h"                                   // Archivo de configuración


/*
* PINES Y VARIABLES
*/   
              
#include <ArduinoJson.h>                              // Para los datos JSON
#include <HTTPClient.h>
#include <Wire.h>                                     // Para interfaz I2C para, comunicaciones de dispositivos por direcciones
#include <RTClib.h>                                   // Para el manejo del modulo RTC
#include <TimeLib.h>                                  // Libreria para gestionar las conversiones de tiempo
#include <Separador.h>                                // Como su propio nombre indica separa cadenas de datos
#include <RunningMedian.h>                            // Hace una lectura precisa de los sensores
#include <DNSServer.h>                                // Va con la libreria de abajo
#include <WiFiManager.h>                              // Interfaz para conectar el modulo a una red WiFi
#include <ESP_DoubleResetDetector.h>                  // Detecta cuando se ha reiniciado el modulo 2 veces en un periodo de tiempo especificado
#include <HTTPUpdate.h>
#include <PubSubClient.h>
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
  
//Objetos
HTTPClient http;
DoubleResetDetector* drd;
RTC_DS3231 rtc;
Separador s;
WiFiManager wifiManager;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
#ifdef pantallaLCD
  LiquidCrystal_I2C lcd(0x27,16,2);
#endif

/*
  Tareas
*/
TaskHandle_t makeLogTask;
QueueHandle_t queue; 
  
void setup(){

  mqttClient.setServer(ipServer.c_str(), 1883);
  mqttClient.setCallback(callback);

  xTaskCreatePinnedToCore(
    makeLog,      // Function that should be called
    "LOG",            // Name of the task (for debugging)
    1000,               // Stack size (bytes)
    NULL,               // Parameter to pass
    0,                  // Task priority
    &makeLogTask,               // Task handle
    1);          // Core you want to run the task on (0 or 1)
  
//Inicializamos las cosas
  Serial.begin(115200);
  WiFi.begin();
  Wire.begin(15, 4);
  #ifdef pantallaLCD
    lcd.begin();
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

//Seteamos pines a HIGH
  digitalWrite(resis,HIGH);
  digitalWrite(bombaRecirculacion,HIGH);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(bombaFrio,HIGH);
  digitalWrite(peltier,HIGH);

//Detectamos si se ha pulsado el reset mientras el inicio para entrar en la configuracion del WiFi

initResetDetector();
checkReset();
  
// Conectar con la red WiFi
  #ifdef debug
    Serial.println("");
    Serial.print("Connecting");
  #endif

  #ifdef pantallaLCD
    printLCD(0, 0, "Conectando WiFi", 1, 0, "");                          
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
    Serial.println(WiFi.localIP());
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

    Serial.println("++++++++++++++++++++++++++++++++");
    Serial.println(         "Cervecero 2.0");
    Serial.println(         "Version: " + currentVersion);
    Serial.println("++++++++++++++++++++++++++++++++");
  #endif
  #ifdef pantallaLCD
    printLCD(0, 0, "Iniciando...", 1, 0, "");
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
          printLCD(0, 0, "Error al obtener", 1, 0, "el ID de placa");
        #endif
        delay(10000);
      }
  }

  checkrecovery();
  if (!recovery){
    
    checkforUpdates();
    
  } else{
    
    leerReceta();
    faseProceso = recoveryPasoProceso;
    recoveryProcesos(recoveryProceso);
    
  }

}

void loop(){
  
 /* 
  * Menu de consultas con PHP MySQL en formato JSON 
  */
  #ifdef new_menu
    #ifdef pantallaLCD
      printLCD(0, 0, "Cervecero v" + currentVersion, 1, 0, " Ready");
    #endif
    #ifdef debug
      Serial.println("------------------------------");
      Serial.println("Ready");
      Serial.println("------------------------------");
  #endif
    json_menu();
    
  #endif
 /*
  * Menu de consultas PHP con MySQL
  */
  #ifdef old_menu
    SQL_menu();
    menuinicio(dato);
  #endif

  #ifdef json_mqtt_menu
    if (!mqttClient.connected()) {
      
      reconnect();
    
    }
    
    mqttClient.loop();
  #endif
}