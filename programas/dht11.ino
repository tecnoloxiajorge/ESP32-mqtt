void dht11_LCD(int temperatura, int humedad) {
  mylcd.clear();
  mylcd.setCursor(0, 0);
  mylcd.print("T = ");
  mylcd.print(temperatura);
  mylcd.print(" C");

  mylcd.setCursor(0, 1);
  mylcd.print("H = ");
  mylcd.print(humedad);
  mylcd.print(" %");
}

void enviar_dht11_MQTT(int temperatura, int humedad) {
  client.publish(TOPIC_TEMPERATURA, String(temperatura).c_str(), true);
  client.publish(TOPIC_HUMEDAD, String(humedad).c_str(), true);
}


void dht11() {
  if (xht.receive(dht)) {
    int humedad = dht[0];
    int temperatura = dht[2];

    if (!isnan(temperatura) && !isnan(humedad)) {
      dht11_LCD(temperatura, humedad);
      enviar_dht11_MQTT(temperatura, humedad);

      if (depuracion) {
        Serial.printf("Temperatura: %d °C | Humedad: %d %%\n", temperatura, humedad);
      }
    } else {
      client.publish(TOPIC_DEPURACION, "[ERROR] Sensor DHT inválido", true);
      Serial.println("[ERROR] Sensor DHT inválido");
    }
  }
}

