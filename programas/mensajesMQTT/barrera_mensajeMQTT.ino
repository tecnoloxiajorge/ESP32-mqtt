#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* ssid = "RedeTecno";
const char* password = "tecnoloxia";

// MQTT
const char* mqtt_server = "192.168.1.102";  // IP de tu broker MQTT (donde está Node-RED)
const int mqtt_port = 1883;

// Inicialización de clientes
WiFiClient espClient;
PubSubClient client(espClient);

// LCD I2C (dirección 0x27 y tamaño 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables para guardar los últimos mensajes
String mensajeTexto = "";
String mensajeFecha = "";

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Esperando...");

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
