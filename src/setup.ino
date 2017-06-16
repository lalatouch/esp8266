#include "led.h"

ESP8266WebServer server(80);

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
		led::on(led::ONBOARD);
		server.send(200);
	});
	server.on("/led/off", []() {
		led::off(led::ONBOARD);
		server.send(200);
	});
	server.on("/led", []() {
		if (server.hasArg("red"))   led::pwm(led::RGB_R, atoi(server.arg("red").c_str()));
		if (server.hasArg("green")) led::pwm(led::RGB_G, atoi(server.arg("green").c_str()));
		if (server.hasArg("blue"))  led::pwm(led::RGB_B, atoi(server.arg("blue").c_str()));
		server.send(200);
	});

	server.begin();
}

