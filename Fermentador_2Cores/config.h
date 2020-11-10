 
  static String currentVersion = "1.0.7";                      // Versión del Firmware
  static String host = "http://192.168.1.150/php/arduino/";    // Servidor de PHP donde manda y recibe información
  static String ipServer = "192.168.1.150";               // Servidor de actualizaciones

/*
 * Configuraciones del programa
 */

// Configuracion DobleResetDetector (no tocar)
  #define DOUBLERESETDETECTOR_DEBUG       false
  #define ESP_DRD_USE_EEPROM      true

// Activar menu nuevo o antiguo 
  //#define new_menu
  //#define old_menu
  #define json_mqtt_menu
  
 
// Activar todos los mensajes por serial
  #define debug

//Activar la pantalla 16x2 LiquidCrystal
  #define pantallaLCD

// Rango para la temperatura
  const float rangoTemp = 1;

// Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int tiempoTrasvase = 210000;

// Tiempo de retraso entre el arranque de la bomba frio y el resto
  const int retrasoBombas = 1000;
