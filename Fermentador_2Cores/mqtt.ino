void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {

    Serial.println("Attempting MQTT connection...");

    // Attempt to connect
    if (mqttClient.connect("2")) {
      #ifdef pantallaLCD
      printLCD(0, 0, "Cervecero v" + currentVersion, 1, 0, " Ready");
      lcd.setCursor(0,1);
      lcd.print(" Ready   Online");
      #endif
      #ifdef debug
        Serial.println("------------------------------");
        Serial.println("Ready");
        Serial.println("------------------------------");
      #endif
      // Once connected, publish an announcement...
      mqttClient.publish("2/state", "online");
      // ... and resubscribe
      mqttClient.subscribe("2/menu");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  
  String datos;
  
  for (int i = 0; i < length; i++) {
    
    datos += (char)payload[i];
    
  }
  
  Serial.println(datos);
  
  json_mqtt_decode(topic, datos);
  
}
