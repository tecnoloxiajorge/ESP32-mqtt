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

// Función para actualizar el LCD
void actualizarLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensajeFecha.substring(0, 16)); // Línea 1: Fecha
  lcd.setCursor(0, 1);
  lcd.print(mensajeTexto.substring(0, 16)); // Línea 2: Texto
}

// Callback MQTT
void callback(char* topic, byte* message, unsigned int length) {
  String payload = "";
  for (int i = 0; i < length; i++) {
    payload += (char)message[i];
  }

  Serial.print("Mensaje recibido en [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(payload);

  if (String(topic) == "barrera1/actuadores/pantalla") {
    mensajeTexto = payload;
  } 
  else if (String(topic) == "barrera1/actuadores/fecha") {
  // Ejemplo: payload = "2025-05-23T11:02"
  String fechaISO = payload;
  int year = fechaISO.substring(2, 4).toInt();      // "25"
  int month = fechaISO.substring(5, 7).toInt();     // "05"
  int day = fechaISO.substring(8, 10).toInt();      // "23"
  String hora = fechaISO.substring(11, 16);         // "11:02"

  // Construimos el formato deseado: "23-5-25 11:02"
  mensajeFecha = String(day) + "-" + String(month) + "-" + String(year) + " " + hora;
}

  actualizarLCD();
}

// Conexión WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// Conexión MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP32Client-";    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado");
      client.subscribe("barrera1/actuadores/pantalla");
       client.subscribe("barrera1/actuadores/fecha");
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

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
