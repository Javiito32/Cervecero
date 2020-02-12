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

/***************************************
 * Define Instances and Constants
 ****************************************/

const char* UBIDOTS_TOKEN = "BBFF-0zWwVurprtGmHpfjoFBLIKPLEGnEB4";  // Put here your Ubidots TOKEN

const char* WIFI_SSID = "Kerjox";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "";      // Put here your Wi-Fi password

const char* DEVICE_LABEL_TO_RETRIEVE_VALUES_FROM = "840d8eb07c53";  // Replace with your device label
const char* VARIABLE_LABEL_TO_RETRIEVE_VALUES_FROM = "ledstate";
Ubidots ubidots(UBIDOTS_TOKEN, UBI_TCP);

/****************************************

//Librerias
#include "Ubidots.h"

//LAYOUT Pines
  const int pinSonda = A0;                    //Sonda de la temperatura
  const int resis = D0;                       //Resistencia para calentar
  const int bombaRecirculacion = D1;          //Bomba de recirculacion 230V
  const int bombaTrasvase = D2;               //Bomba trasvase 230V
  const int bombaFrio = D3;                   //Bomba refrigeracion 230V
  const int peltier = D4;                     //Celulas Peltier
  const int reserva = D5;                     //Este cable se deja en reserva, era para el RELE DE ESTADO SOLIDO que no se usa
  
//Variables configurables
  const float anchoVentana = 1;               //Rango para la temperatura
  const float tiempoTrasvase = 210000;        //Tiempo maximo de seguridad que dura el trasvase (Se pone 4 minutos)
  const int retrasoBombas = 1000;             //Tiempo de retraso entre el arranque de la bomba frio y el resto

//Variables de trabajo
  int sensorTemperatura;                      //Variable que almacen la lectura de la sonda
  char dato;                                  //Dato leido para entrar el menu
  float tmax;                                 //Temperatura maxima para los procesos
  float tmin;                                 //Temperatura minima para los procesos
  float tiempoi;                              //Tiempo inicial para los procesos en milis
  float tiempof;                              //Tiempo final para los procesos en milis
  float tiempoActual;                         //Tiempo actual del proceso en milis
  float tiempoRestante;                       //Tiempo que falta para el final de los procesos en milis
  float tiempoEnviar;                         //Tiempo que se envia a la Rasberry en segundos
  float milivoltios;                          //Variable para almacenar los milivoltios
  float celsius;                              //Variable para almacenar los grados
  int cronometroi;                            //Tiempo inicial para el envio a Rasberry
  int cronometrof;                            //Tiempo final pare el envio a Rasberry
  int cronometro;                             //Tiempo actual para el envio a Rasberry

/*
 * CICLO DE ARRANQUE
 * Configuramos los pines. Puesta a cero inicial.
 */
void setup(){
//Inicializamos el puerto serie
  Serial.begin(115200);
  
//Configuracion de pines
  pinMode(resis,OUTPUT);
  pinMode(bombaRecirculacion,OUTPUT);
  pinMode(bombaTrasvase,OUTPUT);
  pinMode(bombaFrio,OUTPUT);
  pinMode(peltier,OUTPUT);
  pinMode(reserva,OUTPUT);
  
//Puesta a cero inicial de las variables de trabajo
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  tiempoRestante = 0;
  cronometroi = 0;
  cronometrof = 0;
  cronometro = 0;
}

/*
 * CICLO PRINCIPAL
 * Indicamos a la Raspberry que hemos arrancado. 
 * Leemos datos en bucle hasta entrar en un proceso.
 */
void loop(){
//Enviar variables
  //ubidots.add("Nombre variable", %variable%);

//Mensaje inicial
  Serial.println("O0");
   
//Lectura de datos para ejecutar un proceso
  bool bufferSent = false;      
  bufferSent = ubidots.send();           // Will send data to a device label that matches the device Id
  if (bufferSent){
    dato = ubidots.get();
    if(dato>'0' && dato<='9'){ 
      menu(dato);
    }else{
    Serial.println("Los datos no se mandarosn correctamente");
    }
  }
  
//Fin del ciclo
  dato = '0';
  delay(5000);
}

/*
 * Funcion menu.
 * Es el encargado de gestionar el numero de proceso, del 1 al 9, y 
 * elegir el proceso a realizar.
 * Datos del menu:
 *  - 1: Maceracion.
 *  - 2: Coccion.
 *  - 3: Trasvase.
 *  - 4: Fermentacion.
 *  - 5: Reserva.
 *  - 6: Reserva.
 *  - 7: Reserva.
 *  - 8: Reserva.
 *  - 9: Reserva.
 *  
 *  Parametros: numero de proceso introducido como caracter
 *  No devuelve nada
 */
void menu(char n){   
       if (n=='1') { maceracion(); }
  else if (n=='2') { coccion();}
  else if (n=='3') { trasvase();}
  else if (n=='4') { fermentacion();}
  else if (n=='5') { Serial.println("Proceso no existente");}
  else if (n=='6') { Serial.println("Proceso no existente");}
  else if (n=='7') { Serial.println("Proceso no existente");}
  else if (n=='8') { Serial.println("Proceso no existente");}
  else if (n=='9') { Serial.println("Proceso no existente");}
  else {Serial.println("Dato no valido");}
}


/*  
 *  Funcion para realizar MACERACION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  La consigna empieza por "T", seguida de cuatro numeros (centenas de temperatura, decenas de 
 *  temperatura, unidades de temperatura y decimas de temperatura), seguidas por "S" y el tiempo
 *  del proceso en segundos, acabando la consigna en "."
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la constante.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void maceracion (){
//Confirmacion para RASPBERRY del inicio de proceso de maceracion
  Serial.println("O1");
  
//LECTURA DE VARIABLES
  String informacionMaceracion = leer();
  float temperaturaMaceracion = desencriptarTemperatura (informacionMaceracion);      //Variable con la temperatura del proceso
  float tiempoMaceracion = desencriptarTiempo (informacionMaceracion);                //Variable del tiempo del proceso en segundos

//MODO RECIRCULACION
  recircular();
  
//CICLO DE CALENTAMIENTO
  calentar(temperaturaMaceracion,tiempoMaceracion);
  
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(1,0);
  
}

/*
 *  Funcion para realizar COCCION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  La consigna empieza por "T", seguida de cuatro numeros (centenas de temperatura, decenas de 
 *  temperatura, unidades de temperatura y decimas de temperatura), seguidas por "S" y el tiempo
 *  del proceso en segundos, acabando la consigna en "."
 *  Se pone en modo recirculacion y realiza el ciclo de calentamiento hasta la temperatura 
 *  recibida durante el tiempo recibido con un rango de temperatura especificado en la constante.
 *  Encripta las variables y las envia constantemente para que la Raspberry tenga la informacion 
 *  del tiempo restante del proceso.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void coccion (){
//Confirmacion para RASPBERRY del inicio de proceso de maceracion
  Serial.println("O2");
  
//LECTURA DE VARIABLES
  String informacionCoccion = leer();
  float temperaturaCoccion = desencriptarTemperatura (informacionCoccion);      //Variable con la temperatura del proceso
  float tiempoCoccion = desencriptarTiempo (informacionCoccion);                //Variable del tiempo del proceso en segundos

//MODO RECIRCULACION
  recircular();

//CICLO DE CALENTAMIENTO
  calentar(temperaturaCoccion, tiempoCoccion);
  
//APAGADO DE BOMBAS Y RELES
  digitalWrite(resis,LOW);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(bombaFrio,LOW);
  digitalWrite(peltier,LOW);
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(2,0);
}

/*
 *  Funcion para realizar TRASVASE.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso y activa las 
 *  bombas y reles necesarios.
 *  Se pone en modo trasvase hasta que recibe un mensaje de fin que viene dado por la 
 *  siguiente consigna: "T0000S0."
 *    
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void trasvase(){
  boolean fin = false;
//Confirmacion para RASPBERRY del inicio de proceso de trasvase
  Serial.println("O3");
  
//Iniciamos el tiempo
  tiempoi = millis();
  
//Trasvase ON
  digitalWrite(bombaFrio,HIGH);
  delay(retrasoBombas);
  digitalWrite(bombaRecirculacion,LOW);
  digitalWrite(bombaTrasvase,HIGH);
  digitalWrite(peltier,HIGH);
  
//Lectura de orden de fin
  do{
  //Control del proceso por tiempo limite de seguridad
    tiempoActual = millis();
    tiempoRestante = tiempoActual - tiempoi;                                        //Tiempo que llevamos de trasvase
    if(tiempoRestante > tiempoTrasvase){fin = true;}
  //Control del proceso por orden de la Raspberry
    String informacionTrasvase = leer();
    float tiempoTrasvase = desencriptarTiempo (informacionTrasvase);                //Variable del tiempo del proceso en segundos
    if(tiempoTrasvase == 0.00){fin = true;}
  }while(!fin);
  
//Trasvase OFF  
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
  
//Puesta a cero de las variables
  tiempoi = 0;
  tiempoActual = 0;
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(3,0);
}



/*
 *  Funcion para realizar FERMENTACION.
 *  Avisar a la Raspberry de que esta preparado para empezar el proceso. 
 *  Recibe una consigna y la desencripta volcando la temperatura y el tiempo en variables.
 *  Finaliza el proceso cuando acaba el tiempo
 *  Encripta las variables y las envia.
 *  Una vez alcanzado el tiempo envia mensaje de fin.  
 *  
 *  Parametros: No lleva parametros
 *  No devuelve nada
 */
void fermentacion(){
//Confirmacion para RASPBERRY del inicio de proceso de fermentacion
  Serial.println("O4");
  
//LECTURA DE VARIABLES
  String informacionFermentacion = leer();
  float temperaturaFermentacion = desencriptarTemperatura (informacionFermentacion);      //Variable con la temperatura del proceso
  float tiempoFermentacion = desencriptarTiempo (informacionFermentacion);                //Variable del tiempo del proceso en segundos

//CONTADOR DE TIEMPO
  //Tratamiento del tiempo del proceso
    tiempoi = millis();
    tiempoFermentacion = tiempoFermentacion * 1000;                 //Paso del tiempo a milis
    tiempof = tiempoFermentacion + tiempoi;
  //Control de tiempo para los envios por pantalla
    cronometroi = millis();
    cronometrof = 1000 + cronometroi;
    do{
      tiempoActual = millis();
      cronometro = millis();
    //Cuenta atras para sacar por pantalla
      tiempoRestante = tiempof - tiempoActual;
      cronometro = cronometrof - cronometro;
      if(cronometro == 0){
        tiempoEnviar = tiempoRestante/1000;                 //Paso del tiempo a segundos
        enviarTiempo(tiempoEnviar);
        cronometroi = millis();
        cronometrof = 1000 + cronometroi;
      }
    }while(tiempoActual+1 < tiempof);
  
//PUESTA A CERO FINAL
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  tiempoEnviar = 0;
  cronometro = 0;
  cronometroi = 0;
  cronometrof = 0;
  
//Envio a RASPBERRY el mensaje de fin de proceso.
  finProceso(4,0);
}


/*
 * Metodo para recircular. 
 * Sirve poner los reles y bombas en la posicion de recirculacion.
 * 
 * No tiene parametros
 * No devuelve nada
 */
void recircular(){
  digitalWrite(bombaRecirculacion,HIGH);
  digitalWrite(bombaTrasvase,LOW);
  digitalWrite(peltier,LOW);
  digitalWrite(bombaFrio,LOW);
}

/*
 * Metodo calentar. 
 * Sirve realizar un ciclo de calentamiento hasta la temperatura alcanzar la temperatura pasada como parametro
 * Parametros: tiempo del proceso en segundos
 *             temperatura del proceso en grados
 * No devuelve nada
 */
void calentar( float temperaturaProceso, float tiempoProceso){
//TRATAMIENTO DE LAS VARIABLES
  //Tratamiento de la ventana de temperatura
    tmax = temperaturaProceso+anchoVentana;
    tmin = temperaturaProceso-anchoVentana;
  //Tratamiento del tiempo del proceso
    tiempoi = millis();
    tiempoProceso = tiempoProceso * 1000;                 //Paso del tiempo a milis
    tiempof = tiempoProceso + tiempoi;
  //Control de tiempo para los envios por pantalla
    cronometroi = millis();
    cronometrof = 1000 + cronometroi;

//CICLO DE CALENTAMIENTO
  do{
    tiempoActual = millis();
    cronometro = millis();
  //Cuenta atras para sacar por pantalla
    tiempoRestante = tiempof - tiempoActual;
    cronometro = cronometrof - cronometro;
    if(cronometro == 0.0){
      tiempoEnviar = tiempoRestante/1000;                 //Paso del tiempo a segundos
      enviarTiempo(tiempoEnviar);
      cronometroi = millis();
      cronometrof = 1000 + cronometroi;
    }
  //Tratamiento de la temperatura
    sensorTemperatura = analogRead(pinSonda);
    milivoltios = (sensorTemperatura / 1023.0) * 3300;
    celsius = milivoltios / 10;
 
  //Mantenimiento de la ventana de temperatura
    if(celsius > tmax){digitalWrite(resis,LOW);}
    if(celsius < tmin){digitalWrite(resis,HIGH);}
  }while(tiempoActual+1 < tiempof);
  
//PUESTA A CERO FINAL
  tmax = 0;
  tmin = 0;
  tiempoi = 0;
  tiempof = 0;
  tiempoActual = 0;
  cronometro = 0;
  cronometroi = 0;
  cronometrof = 0;
}


/*
 * Metodo leer. 
 * Sirve para leer una cadena de caracteres que se recibe por el puerto
 * serie y se almacena en un String. Dejará de leer cuando reciba un punto.
 * 
 * No tiene parametros
 * Devuelve la cadena leida.
 */
String leer(){
//VARIABLES LOCALES
  char caracterLeido;
  char caracterViejo;
  String cadenaLeida = "";
  String cadenaVieja = "";
  boolean lectura = true;
  boolean estado = true;
  
//BUCLE DE LECTURA
  while(lectura){
  //Lee si esta disponible
    if(Serial.available()>0){
      caracterLeido = Serial.read();
      //Deja de leer cuando se encuentra con un punto
      if(caracterLeido == '.'){lectura = false;}
      //Tratamiento de la cadena
      cadenaVieja = cadenaLeida;
      cadenaLeida += caracterLeido;
      estado = cadenaVieja.compareTo(cadenaLeida);
      if(estado == false){cadenaLeida = cadenaVieja;}
    }
  }//Fin del bucle de lectura
  
//DEVOLUCION DE LA CADENA LEIDA
  return cadenaLeida;
}

/*
 * Metodo desencriptarTemperatura.
 * Almacena en una cadena los carácteres que contienen el dato de la temperatura
 * y dicho String lo convierte a número.
 * 
 * Parametros: Cadena de carácteres a tratar
 * Devuelve el valor de la temperatura con decimales
 */
float desencriptarTemperatura (String cadena){
//Almacenamiento de la subcadena correspondiente a la temperatura
  String temperatura = cadena.substring(1,6);
  
//Conversión de String a número
  float valorTemperatura = temperatura.toFloat();
  
//Tratamiento del número
  valorTemperatura = valorTemperatura/10;
  
//Devolución del valor de temperatura
  return valorTemperatura;
}

/*
 * Metodo desencriptarTiempo.
 * Almacena en una cadena los carácteres que contienen el dato del tiempo
 * y dicho String lo convierte a número.
 * 
 * Parametros: Cadena de carácteres a tratar
 * Devuelve el valor del tiempo en segundos
 */
float desencriptarTiempo (String cadena){
//Almacenamiento de la subcadena correspondiente al tiempo
  String tiempo = cadena.substring(6,cadena.length());
  
//Conversion de String a numero
  float valortiempo = tiempo.toFloat();
  
//Devolucion del valor de tiempo
  return valortiempo;
}


/*
 * Metodo enviarTiempo.
 * Convierte el tiempo pasado como parametro a un String que muestra
 * por el puerto Serie (para ser recibido por la Rasberry)
 * 
 * Parametros: Tiempo
 * No devuelve nada
 */
void enviarTiempo (float tiempoRestante){
//Variables locales
  String mensaje = "S";
  unsigned long tiempo;
  
//Conversion a String
  tiempo = tiempoRestante;
  mensaje.concat(tiempo);
  
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}


/*
 * Metodo para enviar el final del proceso con errores.
 * Envia un mensaje a la Rasberry con los errores del proceso.
 * 
 * Parametros: dato  Representa el numero de proceso
 *             error Representa el numero de error (0 si no hay)
 * No devuelve nada
 */
void finProceso (int dato,int error){
//Variables locales
  String mensaje = "O";
  
//Conversion a String
  mensaje.concat(dato);
  mensaje.concat("F");
  mensaje.concat(error);
  
//Envia el string por a la Raspberry
  Serial.println(mensaje);
}
