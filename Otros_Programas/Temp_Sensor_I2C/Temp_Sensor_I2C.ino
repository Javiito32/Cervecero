//SLAVE

#define DEVICEID 9
#define MEASURES 5
#define SONDA A0
//#define DEBUG

#include <Wire.h>
#include <RunningMedian.h>

RunningMedian samples = RunningMedian(MEASURES);

char temp[6]; //2 int, 2 dec, 1 point, and \0
char msg[5];

void setup() {
  #ifdef DEBUG
  Serial.begin(9200);
  #endif

  pinMode(SONDA, INPUT);
  
  Wire.begin();

  Wire.begin(DEVICEID);
  // Set the callback to call when data is requested.
  Wire.onRequest(requestCallback);
}
void loop() {

  //float stempC = 20.00;
  float stempC = (5.0 * analogRead(SONDA) * 100.0) / 1024.0;
 
//get the temp/humid into chars to format
  for (uint8_t i = 0; i < MEASURES; i++) {

    samples.add(stempC);
  }

  stempC = samples.getAverage();
  dtostrf(stempC, 5, 2, temp);
  Serial.println(temp);
  sprintf(msg,"%s",temp);
  #ifdef DEBUG
  Serial.println(msg);
  #endif
  delay(1000);
}
void requestCallback() {
  
  Wire.write(msg);
}
