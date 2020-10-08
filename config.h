/*
 * Configuraciones del programa
 */

// Activar menu nuevo o antiguo 
  #define new_menu
  
  //#define old_menu
  
 
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
