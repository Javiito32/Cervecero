#define LUCECITA              5
#define ceroPulse             18
#define TIMER1_INTERVAL_MS    1

#include "ESP32TimerInterrupt.h"

byte ticks = 0;
bool SoloUnPulsito;
byte disparoEnPulso = 0;
bool direccion = false;

void IRAM_ATTR TimerHandler1(){
  
  //Serial.print("ITimer1: millis() = "); Serial.println(millis());

  static bool started = false;

  if (!started) {
    started = true;
    pinMode(LUCECITA, OUTPUT);
  }
  
  ticks++;
  //Serial.println(ticks);

  if(ticks >= disparoEnPulso && SoloUnPulsito) {
        
    digitalWrite(LUCECITA, HIGH);
    delayMicroseconds(1000); //Para arduino uno un pulsito de 10us para el NodeMCU 15us
    digitalWrite(LUCECITA, LOW);
    //Serial.println("Disparo");
    SoloUnPulsito = false;
        
  } else {digitalWrite(LUCECITA, LOW);};
  
}

ESP32Timer ITimer1(1);


void IRAM_ATTR interrupt() {

  while(digitalRead(ceroPulse) != LOW){
    
  }
  //delayMicroseconds(1000);
  ticks = 0;
  SoloUnPulsito = true;
  //Serial.println("Interruption");
  ITimer1.restartTimer();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ceroPulse, INPUT);
  attachInterrupt(ceroPulse, interrupt, CHANGE);


  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

}

void loop() {
  // put your main code here, to run repeatedly:
  if (disparoEnPulso >= 10) {
    direccion = false;
  } else if (disparoEnPulso <= 0) {
    direccion = true;
  }
    if(direccion) {
      disparoEnPulso++; 
    }else {
      disparoEnPulso--; 
    }
   delay(200);
}
