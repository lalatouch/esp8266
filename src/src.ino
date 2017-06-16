#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

#include "led.h"

extern ESP8266WebServer server;

void setup() {
	// Init serial (debugging)
	Serial.begin(115200);
	Serial.println("Booting");

	// Setup LEDs
	led::setup();
	// Setup manager
	setupAP();
	// Setup OTA
	setupOTA();
	// Setup I2C
	setupI2C();
	// Setup HTTP
	setupHTTPServer();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void loop() {
	ArduinoOTA.handle();
	server.handleClient();
}

