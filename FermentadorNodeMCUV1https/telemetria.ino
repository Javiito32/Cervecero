void sendInfo(int proceso,byte pasoProceso) {
  Serial.println("Log");
  if (WiFi.status() == WL_CONNECTED) {
    String datos_Enviar = "IDplaca=";
    datos_Enviar.concat(IDplaca);
    datos_Enviar.concat("&receta=");
    datos_Enviar.concat(IDreceta);
    datos_Enviar.concat("&proceso=");
    datos_Enviar.concat(proceso);
    datos_Enviar.concat("&pasoProceso=");
    datos_Enviar.concat(pasoProceso);
    datos_Enviar.concat("&estado=");
    datos_Enviar.concat(estado);
    datos_Enviar.concat("&tiempoRestante=");
    datos_Enviar.concat(tiempoRestante);
    datos_Enviar.concat("&porcentaje="); 
    datos_Enviar.concat(porcentaje);
    peticion("info.php",datos_Enviar);
  }
}
