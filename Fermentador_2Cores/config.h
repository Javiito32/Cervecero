
  static String currentVersion = "0.0.6";                      // Versión del Firmware
  static String host = "http://192.168.1.150/php/arduino/";    // Servidor de PHP donde manda y recibe información
  static String ipServer = "192.168.1.150";               // Servidor de actualizaciones

/*
 * Configuraciones del programa
 */

// Configuracion DobleResetDetector (no tocar)
  #define DOUBLERESETDETECTOR_DEBUG       false
  #define ESP_DRD_USE_EEPROM      true
  #define SECONDSTOENTER  3
  
// Activar todos los mensajes por serial
  #define ENABLE_SERIAL

//Activar la pantalla 16x2 LiquidCrystal
  #define pantallaLCD

//LAYOUT Pines
#define LED_BUILTIN 25
#define SONDA 39                                   // Sonda de la temperatura
#define HEATER 14                                      // Resistencia para calentar               
#define TUBESENSOR 34                              // Sensor de liquido en tubo
#define bombaPrincipal 27                         // Bomba de recirculacion 230V
#define bombaFrio 32                                  // Bomba refrigeracion 230V
#define electroRecirculacion 17
#define electroTrasvase 33                              // Bomba trasvase 230V

#define DRD_TIMEOUT 10                                // El tiempo en segundos que va a esperar para el doble reset
#define DRD_ADDRESS 0                                 // RTC Memory Address for the DoubleResetDetector to use

#define SDA_I2C 15
#define SCL_I2C 4

// Rango para la temperatura
  const float rangoTemp = 2;

// Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int tiempoTrasvase = 210000;

// Tiempo de retraso entre el arranque de la bomba frio y el resto
  const int retrasoBombas = 1000;
