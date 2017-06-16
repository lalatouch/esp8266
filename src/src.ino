#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "led.h"
#include "wifi.h"
#include "i2c.h"

extern ESP8266WebServer server;

void setup() {
	// Init serial (debugging)
	Serial.begin(115200);
	Serial.println("Booting");

	// Setup LEDs
	led::setup();
	// Setup manager and OTA
	wifi::setup();
	// Setup I2C
	i2c::setup();
	// Setup HTTP
	setupHTTPServer();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void loop() {
	wifi::handleOTA();
	server.handleClient();
}

