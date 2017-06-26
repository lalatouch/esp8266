#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

#include "wifi.h"

namespace wifi {

// UDP thingy
WiFiUDP udp;

// Base station info
BaseStation base = {};

void setup() {
	// Sets up an AP if it could not connect to the last network
	WiFiManager wifiManager;
	wifiManager.autoConnect("LalaTouch", "LalaThing");

	// Port defaults to 8266
	// ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// ArduinoOTA.setHostname("myesp8266");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR)         Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR)     Serial.println("End Failed");
	});
	ArduinoOTA.begin();

	// Setup UDP connectivity
	udp.begin(1415); // improved lala port (only the worthy will understand)

	// Wait to receive a one-byte packet containing 42
	uint8_t received = 0;
	while (received != 42) {
		while (udp.parsePacket() != 1);

		if (udp.read(&received, 1) == 0) received = 0;
	}

	// Set the base station info
	base.address = udp.remoteIP();
	base.port = 1414;

	Serial.print("Base station has IP address ");
	Serial.println(base.address.toString());
}

void handleOTA() {
	ArduinoOTA.handle();
}

void send(const uint8_t *buffer, const size_t size) {
	udp.beginPacket(base.address, base.port);
	udp.write(buffer, size);
	udp.endPacket();
}

}

