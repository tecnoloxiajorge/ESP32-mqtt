# ESP32_mqtt_node-red
Programa para conectar ESP32 a node-red a través de MQTT
## Programa principal
```java
// =======================
//     LIBRERIAS
// =======================
#include <WiFi.h>                 // Conexión WiFi para ESP32
#include <PubSubClient.h>         // Comunicación con servidor MQTT
#include <Wire.h>                 // Comunicación I2C
#include <LiquidCrystal_I2C.h>    // Control de LCD I2C
#include "xht11.h"                // Sensor DHT11 (personalizado)
#include <ESP32servo.h>          // Servomotor

// =======================
//     DEFINICIONES
// =======================

// WiFi
const char* ssid     = "RedeTecno";
const char* password = "tecnoloxia";

// MQTT
#define MQTT_SERVER      "172.20.10.3"

// TOPICOS MQTT
#define TOPIC_TEMPERATURA "barrera1/sensores/temperatura"
#define TOPIC_HUMEDAD     "barrera1/sensores/humedad"
#define TOPIC_LDR          "barrera1/sensores/ldr"
#define TOPIC_DEPURACION  "depuracion"
#define TOPIC_PANTALLA    "barrera1/actuadores/pantalla"
#define TOPIC_FECHA    "barrera1/actuadores/fecha"
#define TOPIC_HORA    "barrera1/actuadores/hora"

// Pines de los sensores y actuadores
#define DHT_PIN 17
#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 18
#define LDR_PIN 35
#define ROJO_PIN 14
#define ANMARILLO_PIN 27
#define VERDE_PIN 16


// Tiempo de muestreo dht11 y pantalla
#define INTERVALO_DHT11 60000         // 60 segundos
#define INTERVALO_PANTALLA 10000     //10 segundos

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

  //ultrasonidos
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // leds
  pinMode(ROJO_PIN, OUTPUT);
  pinMode(AMARILLO_PIN, OUTPUT);
  pinMode(VERDE_PIN, OUTPUT);

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
```
