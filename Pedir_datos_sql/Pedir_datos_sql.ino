#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "MOVISTAR_XD";
const char* password = "845CDE655563B9808EB7";

void setup () {

  Serial.begin(115200); // Start the serial monitor.
  delay(10); // Give it a moment.


  pinMode(2, OUTPUT); // Set GPIO2 as an OUTPUT.
  digitalWrite(2, 0); // Start off.

  // Connect to WiFi network:
  Serial.println("Hello Digital Craft");
  Serial.println("Connecting ");
  WiFi.begin(ssid, password);

  // Show ... until connected:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  // Print the IP address of the device:
  Serial.println(WiFi.localIP());

}

void loop() {

  // Verfiy WiFi is connected:
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;  // Object of the class HTTPClient.

    http.begin("http://192.168.1.150/pedirdatos.php?id=1");  // Request destination.
    int httpCode = http.GET(); // Send the request.

    if (httpCode > 0) { //Check the returning code

      String datos = http.getString();   // Get the text from the destination (1 or 0).
      int longitud = datos.length();
      int ptemp = datos.indexOf("temp="); //Posicion de temp
      String stemp = "";
      for (int i = ptemp + 5; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else stemp += datos[i];
      }
      float temp = stemp.toFloat();
      Serial.print("Temperatura= ");
      Serial.println(temp);

      int phum = datos.indexOf("hum=");
      String shum = "";
      for (int i = phum + 4; i < longitud; i ++){
        if (datos[i] == ';') i = longitud;
        else shum += datos[i];
      }
      float hum = shum.toFloat();
      Serial.print("Humedad= ");
      Serial.println(hum);
      
      

    }else{

      Serial.println("Something baaaaaaad happened!");

    }

    http.end();   //Close connection

  }

  delay(1000);    //Send a request every 30 seconds

}
