
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
#include "./classes/Recipe.h"


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

//Variables globales
String mac;                                           // La direccion MAC de el WiFi
int id_Board;                                          // Identificador unico de la placa, es un número
bool recovery;                                        // 0 - Nada que recuperar, 1 - Recupera el proceso que estubiera haciendo
int recoveryTiempoRestante;                           // Variable de recovery
byte recoveryProceso;                                  // Variable de recovery
byte recoveryPasoProceso;                              // Variable de recovery
byte faseProceso;                                     // El paso del proceso que se esta ejecutando

  
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
Recipe Recipe;

/*
  Tareas
*/
TaskHandle_t makeLogTask;
QueueHandle_t queue; 
  
void setup(){

  mqttClient.setServer(ipServer.c_str(), 1883);
  mqttClient.setCallback(callback);

  /*xTaskCreatePinnedToCore(
    makeLog,      // Function that should be called
    "LOG",            // Name of the task (for debugging)
    10000,               // Stack size (bytes)
    NULL,               // Parameter to pass
    0,                  // Task priority
    &makeLogTask,               // Task handle
    1);          // Core you want to run the task on (0 or 1)*/
  
//Inicializamos las cosas
  #ifdef ENABLE_SERIAL
    Serial.begin(115200);
  #endif

  WiFi.begin();
  Wire.begin(SDA_I2C, SCL_I2C);
  #ifdef pantallaLCD
    lcd.begin();
  #endif
  delay(10);
  
//Configuracion de pines
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(resis, OUTPUT);
  pinMode(bombaRecirculacion, OUTPUT);
  pinMode(bombaTrasvase, OUTPUT);
  pinMode(bombaFrio, OUTPUT);
  pinMode(peltier, OUTPUT);
  pinMode(sensorLiquido, INPUT);
  pinMode(zumbador, OUTPUT);

//Seteamos pines a HIGH
  digitalWrite(resis, HIGH);
  digitalWrite(bombaRecirculacion, HIGH);
  digitalWrite(bombaTrasvase, HIGH);
  digitalWrite(bombaFrio, HIGH);
  digitalWrite(peltier, HIGH);

//Detectamos si se ha pulsado el reset mientras el inicio para entrar en la configuracion del WiFi

initResetDetector();
checkReset();
  
// Conectar con la red WiFi
  Serial.println("");
  Serial.print("Connecting");

  #ifdef pantallaLCD
    printLCD(0, 0, "Conectando WiFi", 1, 0, "");                          
  #endif
  

  while (WiFi.status() != WL_CONNECTED) {               // Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
  }
  mac = WiFi.macAddress();
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
  #ifdef pantallaLCD
    printLCD(0, 0, "Iniciando...", 1, 0, "");
  #endif
  
  /*                          
   * Obtenemos el ID de placa asociado a la MAC
   */
  while (true){
    String datos = peticion("get_id.php","mac=" + mac);
      if (datos != "fallo") {
        id_Board = datos.toInt();
        Serial.println("------------------------------");
        Serial.print("El ID de la placa es el: ");
        Serial.println(id_Board);
        Serial.println("------------------------------");
        break;
      }else{
        Serial.println("------------------------------");
        Serial.println("No se pudo obtener el ID de placa o no está registrada");
        Serial.println("------------------------------");
        #ifdef pantallaLCD
          printLCD(0, 0, "Error al obtener", 1, 0, "el ID de placa");
        #endif
        delay(10000);
      }
  }

  reconnect();
  checkRecovery();
  if (!recovery) {
    
    checkforUpdates();
    homeMessage();
    
  }else {
    
    leerReceta(Recipe.getRecipe());
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

    Serial.println("------------------------------");
    Serial.println("Ready");
    Serial.println("------------------------------");
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
    if (!mqttClient.loop()) {
      
      reconnect();
    
    }
  #endif
}