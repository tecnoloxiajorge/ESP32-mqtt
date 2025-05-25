// Función para mostrar el mensaje en el LCD
void mostrarEnLCD(String mensaje) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mensaje MQTT:");
  lcd.setCursor(0, 1);
  lcd.print(mensaje.substring(0, 16)); // Solo los primeros 16 caracteres


// Callback MQTT
void callback(char* topic, byte* message, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)message[i];
  }

  Serial.print("Mensaje recibido: ");
  Serial.println(mensaje);

  mostrarEnLCD(mensaje);
}