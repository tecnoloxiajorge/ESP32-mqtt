#include <ESP32Servo.h>
Servo barrera;  // create servo object to control a servo

int servoPin = 13;
                
void setup() {
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	barrera.setPeriodHertz(50);    // standard 50 hz servo
	barrera.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

}

void loop() {
	barrera.write(0);    // cierra la barrera
	delay(2000);
	barrera.write(180);    // abre la barrera
	delay(4000);
}
