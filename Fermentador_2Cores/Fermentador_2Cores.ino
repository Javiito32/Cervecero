
/*
 *  Notas de la version 1.0.0:
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
#include "PCF8574.h"
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
#include "heltec.h"
#include "images.cpp"

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
PCF8574 expander(0x20);
RTC_DS3231 rtc;
Separador s;
WiFiManager wifiManager;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
RunningMedian samples = RunningMedian(10);
#ifdef pantallaLCD
  LiquidCrystal_I2C lcd(0x27,16,2);
#endif
Recipe Recipe;

IPAddress local_IP(192, 168, 1, 184);
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
  
void setup(){

  mqttClient.setServer(ipServer.c_str(), 1883);
  mqttClient.setCallback(callback);
  
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
  pinMode(SONDA, INPUT);
  pinMode(TUBESENSOR, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(HEATER, OUTPUT);
  expander.pinMode(bombaPrincipal, OUTPUT);
  expander.pinMode(bombaFrio, OUTPUT);
  expander.pinMode(electroRecirculacion, OUTPUT);
  expander.pinMode(electroTrasvase, OUTPUT);

//Seteamos pines a HIGH
  digitalWrite(HEATER, HIGH);
  expander.digitalWrite(bombaPrincipal, HIGH);
  expander.digitalWrite(bombaFrio, HIGH);
  expander.digitalWrite(electroRecirculacion, HIGH);
  expander.digitalWrite(electroTrasvase, HIGH);


//Detectamos si se ha pulsado el reset mientras el inicio para entrar en la configuracion del WiFi

  initResetDetector();
  checkReset();
  
// Conectar con la red WiFi
  Serial.println("");
  Serial.print("Connecting");

  #ifdef pantallaLCD
    printLCD(0, 0, "Conectando WiFi", 1, 0, "");                          
  #endif

  uint8_t cont = 0;

  while (WiFi.status() != WL_CONNECTED) {               // Mostrar ... mientras se conacta al WiFi
    delay(500);
    Serial.print(".");
    if (cont == 5){
      printLCD(0, 6, "Error WiFi", 1, 0, "");  
    }else cont++;
    
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

  //checkforUpdates();
  reconnect();
  checkRecovery();
  if (!recovery) {
    
    homeMessage();
    
  }else {
    
    leerReceta(Recipe.getRecipe());
    faseProceso = recoveryPasoProceso;
    recoveryProcesos(recoveryProceso);
    
  }

}

void loop(){

    if (!mqttClient.loop()) {

      reconnect();
    }
}
