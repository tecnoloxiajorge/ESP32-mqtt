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
#include <ESP32_Servo.h>          // Servo barrera

// =======================
//     DEFINICIONES
// =======================

// WiFi
const char* ssid     = "RedeTecno";
const char* password = "tecnoloxia";

// MQTT
#define MQTT_SERVER      "192.168.1.102"

// Cambiar en cada grupo!!!
#define TOPIC_TEMPERATURA "barrera1/sensores/temperatura"
#define TOPIC_HUMEDAD     "barrera1/sensores/humedad"
#define TOPIC_DISTANCIA   "barrera1/sensores/distancia"
#define TOPIC_LDR         "barrera1/sensores/luminosidad"
#define TOPIC_PANTALLA    "barrera1/actuadores/pantalla"
#define TOPIC_LED         "barrera1/actuadores/led"
#define TOPIC_DEPURACION  "depuracion"


// Pines de los sensores y actuadores
#define DHT_PIN 17
#define SERVO_PIN 18
#define TRIG_PIN 13
#define ECHO_PIN 12 
#define ROJO_PIN 14
#define VERDE_PIN 27
#define AMARILLO_PIN 16
#define LDR_PIN 35

// Tiempos de muestreo 
#define INTERVALO_MUESTREO 60000
#define INTERVALO_PANTALLA 10000

// =======================
//     OBJETOS GLOBALES
// =======================

LiquidCrystal_I2C mylcd(0x27, 16, 2); // Dirección del LCD, 16 columnas y 2 filas
xht11 xht(DHT_PIN);
WiFiClient espCliente;
PubSubClient client(espCliente);
Servo barrera;

// =======================
//     VARIABLES GLOBALES
// =======================

bool depuracion = true;
unsigned char dht[4] = {0, 0, 0, 0};
unsigned long milisAnteriores = 0;
String porcentaje;
String mensajeTexto = "";
String mensajeFecha = "";
unsigned long ultimoCambioPantalla = 0;
int pantallaActual = 0;

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
  
  pinMode(TRIG_PIN , OUTPUT); // set trigPin to output mode
  pinMode(ECHO_PIN , INPUT); // set echoPin to input mode
  pinMode(ROJO_PIN , OUTPUT); // set trigPin to output mode
  pinMode(VERDE_PIN , OUTPUT); // set trigPin to output mode
  pinMode(AMARILLO_PIN , OUTPUT); // set trigPin to output mode

  //Servo  
  barrera.attach(SERVO_PIN); 
  dht11();
  luminosidad();
}

// =======================
//     LOOP
// =======================
void loop() {
  mostrarPantallaTemporizada();
  client.loop();
  if (!client.connected()) {
    reconnect();
  }

  // Medición de distancia y servo
  distancia();

  // Lectura periódica del DHT11
  unsigned long tiempoActual = millis();
  if (tiempoActual - milisAnteriores >= INTERVALO_MUESTREO) {
    milisAnteriores = tiempoActual;
    dht11();
    luminosidad();
  }
  if (tiempoActual - ultimoCambioPantalla >= INTERVALO_PANTALLA) {
    ultimoCambioPantalla = millis();
    mostrarPantalla();
  }
}
```
