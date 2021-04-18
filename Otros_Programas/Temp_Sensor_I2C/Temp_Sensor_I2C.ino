//SLAVE
#define DEVICEID 9
#define SONDA 5
#define DEBUG

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWireBus(SONDA);
DallasTemperature sensor(&oneWireBus);

char temp[6]; //2 int, 2 dec, 1 point, and \0
char msg[5];

void setup() {
  
  #ifdef DEBUG
  Serial.begin(9200);
  #endif
  Wire.begin();
  sensor.begin(); 

  Wire.begin(DEVICEID);
  // Set the callback to call when data is requested.
  Wire.onRequest(requestCallback);
}
void loop() {
  
  sensor.requestTemperatures();
  
  float stempC = sensor.getTempCByIndex(0);
  
  dtostrf(stempC, 5, 2, temp);
  sprintf(msg,"%s",temp);
  #ifdef DEBUG
  Serial.println(msg);
  #endif
  delay(1000);
}
void requestCallback() {
  
  Wire.write(msg);
}
