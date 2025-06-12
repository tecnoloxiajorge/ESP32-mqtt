void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi: ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado.");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("¡Conectado!");
      client.subscribe(TOPIC_PANTALLA);
      client.subscribe(TOPIC_FECHA);
    } else {
      Serial.print("Fallo: ");
      Serial.print(client.state());
      Serial.println(" - Reintentando en 5s...");
      delay(5000);
    }
  }
}
