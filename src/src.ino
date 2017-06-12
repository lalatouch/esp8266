#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

void setup() {
	// Init serial (debugging)
	Serial.begin(115200);
	Serial.println("Booting");

	// Setup manager
	setupAP();
	// Setup OTA
	setupOTA();
	// Setup I2C
	setupI2C();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void loop() {
	ArduinoOTA.handle();
}
