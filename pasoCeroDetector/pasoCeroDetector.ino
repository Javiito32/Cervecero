
#define lucecita 5
#define ceroPulse 34
volatile byte ticks = 0;
byte contarMS = 5;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR interrupt();
void IRAM_ATTR contar();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ceroPulse, INPUT);
  pinMode(lucecita, OUTPUT);
  attachInterrupt(ceroPulse, interrupt, CHANGE);
  Serial.println("Online");

  timer = timerBegin(0, 240, true);
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
}

void IRAM_ATTR interrupt() {
  
  noInterrupts();
  delayMicroseconds(1000);
  ticks = 0;
  Serial.println("Alcachofa");
  interrupts();
}

void IRAM_ATTR contar() {

  ticks++;
  
  if(ticks >= contarMS) {
      
    digitalWrite(lucecita, HIGH);
    delayMicroseconds(15); //Para arduino uno un pulsito de 10us para el NodeMCU 15us
    digitalWrite(lucecita, LOW);
   } 
   //timer1_write(5000);
}

//detachInterrupt(button1.PIN);
