// =======================
//     LIBRERIAS
// =======================
#include <WiFi.h>                 // Conexión WiFi para ESP32
#include <PubSubClient.h>         // Comunicación con servidor MQTT
#include <Wire.h>                 // Comunicación I2C
#include <LiquidCrystal_I2C.h>    // Control de LCD I2C
#include "xht11.h"                // Sensor DHT11 (personalizado)

// =======================
//     DEFINICIONES
// =======================

// WiFi
const char* ssid     = "RedeTecno";
const char* password = "tecnoloxia";

// MQTT
#define MQTT_SERVER      "xxx.xx.xx.xx"

// Cambiar en cada grupo!!!
#define TOPIC_TEMPERATURA "barrera1/sensores/temperatura"
#define TOPIC_HUMEDAD     "barrera1/sensores/humedad"
#define TOPIC_DEPURACION  "depuracion"
#define TOPIC_PANTALLA    "barrera1/actuadores/pantalla"

// Pines de los sensores y actuadores
#define DHT_PIN 17

// Tiempo de muestreo dht11 (60 segundos)
#define INTERVALO_DHT11 60000

// =======================
//     OBJETOS GLOBALES
// =======================

LiquidCrystal_I2C mylcd(0x27, 16, 2); // Dirección del LCD, 16 columnas y 2 filas
xht11 xht(DHT_PIN);
WiFiClient espCliente;
PubSubClient client(espCliente);

// =======================
//     VARIABLES GLOBALES
// =======================

bool depuracion = true;
unsigned char dht[4] = {0, 0, 0, 0};
unsigned long milisAnteriores = 0;

// =======================
//     COMIENZA PROGRAMA
// =======================
void setup() {
  Serial.begin(115200);
  mylcd.init();
  mylcd.backlight();

  conectarWiFi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}

// =======================
//     LOOP
// =======================
void loop() {
  client.loop();

  if (!client.connected()) {reconnect(); }
  
  unsigned long tiempoActual = millis();

  if (tiempoActual - milisAnteriores >= INTERVALO_DHT11) {
    milisAnteriores = tiempoActual;
    dht11();
  }
}

