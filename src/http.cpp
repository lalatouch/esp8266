#include <ESP8266WebServer.h>

#include "http.h"
#include "led.h"

namespace http {

ESP8266WebServer server(80);

void setup() {
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

void handle() {
	server.handleClient();
}

}

