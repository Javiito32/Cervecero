#define LUCECITA              5
#define disparoEnPulso        4
#define ceroPulse             34
#define TIMER1_INTERVAL_MS    1

<<<<<<< HEAD
#include "ESP32TimerInterrupt.h"
=======
byte ticks = 0;
byte disparoEnPulso = 2;
bool SolounPulsito;
>>>>>>> 991781b6d23d2a77bc0c16fee72111d9857de01c

byte ticks = 0;
bool SoloUnPulsito;

void IRAM_ATTR TimerHandler1(){
  
  //Serial.print("ITimer1: millis() = "); Serial.println(millis());

<<<<<<< HEAD
  static bool started = false;

  if (!started) {
    started = true;
    pinMode(LUCECITA, OUTPUT);
  }
  
  ticks++;
  //Serial.println(ticks);

  if(ticks >= disparoEnPulso && SoloUnPulsito) {
        
    digitalWrite(LUCECITA, HIGH);
    delayMicroseconds(5000); //Para arduino uno un pulsito de 10us para el NodeMCU 15us
    digitalWrite(LUCECITA, LOW);
    //Serial.println("Disparo");
    SoloUnPulsito = false;
        
  } else {digitalWrite(LUCECITA, LOW);};
  
=======
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ceroPulse, INPUT);
  pinMode(lucecita, OUTPUT);
  attachInterrupt(ceroPulse, interrupt, CHANGE);
  Serial.println("Online");

  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &contar, true);
  timerAlarmWrite(timer, 1000, true);
  timerAlarmEnable(timer);
/*
  timer1_attachInterrupt(contar);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);  
  timer1_write(5000); // Lanzamos el siguiente evento de tiempo para que dispare en 1ms
*/
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  disparoEnPulso++;
  if (disparoEnPulso>=10){disparoEnPulso=0;}
>>>>>>> 991781b6d23d2a77bc0c16fee72111d9857de01c
}

ESP32Timer ITimer1(1);


void IRAM_ATTR interrupt() {
<<<<<<< HEAD

  while(digitalRead(ceroPulse) != LOW){
    
  }
  //delayMicroseconds(1000);
=======
  
  delayMicroseconds(1000);
>>>>>>> 991781b6d23d2a77bc0c16fee72111d9857de01c
  ticks = 0;
  SoloUnPulsito = true;
  //Serial.println("Interruption");
<<<<<<< HEAD
  ITimer1.restartTimer();
=======
  
>>>>>>> 991781b6d23d2a77bc0c16fee72111d9857de01c
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

}
