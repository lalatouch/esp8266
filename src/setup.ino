#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>

ESP8266WebServer server(80);

void setupLEDs() {
	pinMode(2, OUTPUT);
	digitalWrite(2, LOW);

	for (int i = 12; i <= 14; i++) {
		pinMode(i, OUTPUT);
		analogWrite(i, 0);
	}
}

/**
 * This will automatically setup an AP with captive portal asking you for the
 * wifi network you want to connect to.
 */
void setupAP() {
	WiFiManager wifiManager;
	wifiManager.autoConnect("LalaTouch", "LalaThing");
}
void setupOTA() {
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
}

void setupI2C() {
	// Begin I2C bus
	// GPIO4: SDA, GPIO5: SCL
	Wire.begin(4, 5);
}

void setupHTTPServer() {
	// Root route to test stuff
	server.on("/", []() {
		server.send(200, "text/plain", "Hello world!");
	});

	// LED control
	server.on("/led/on", []() {
		digitalWrite(2, LOW);
		server.send(200);
	});
	server.on("/led/off", []() {
		digitalWrite(2, HIGH);
		server.send(200);
	});
	server.on("/led", []() {
		if (server.hasArg("red"))   analogWrite(12, atoi(server.arg("red").c_str()));
		if (server.hasArg("green")) analogWrite(13, atoi(server.arg("green").c_str()));
		if (server.hasArg("blue"))  analogWrite(14, atoi(server.arg("blue").c_str()));
		server.send(200);
	});

	server.begin();
}

