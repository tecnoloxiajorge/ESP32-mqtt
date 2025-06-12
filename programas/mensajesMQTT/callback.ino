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
