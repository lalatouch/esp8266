#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "wifi.h"
#include "http.h"
#include "led.h"
#include "utils.h"

namespace http {

ESP8266WebServer server(80);

// API base URL
String apiBaseURL;

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

	// Get API IP addr
	apiBaseURL = "http://" + wifi::base.address.toString() + ":3000/api";

	// Self-register to the API
	client::post("/modules/register", "\"" + utils::toHexString(ESP.getChipId()) + "\"");

	Serial.println("Chip ID: " + utils::toHexString(ESP.getChipId()));
}

void handle() {
	server.handleClient();
}

namespace client {

void get(String path) {
	HTTPClient http;
	http.begin(apiBaseURL + path);
	http.GET();
	http.end();
}

void post(String path, String data) {
	HTTPClient http;
	http.begin(apiBaseURL + path);
	http.addHeader("Content-type", "application/json");
	http.POST((uint8_t*)data.c_str(), data.length());
	http.end();
}

}

}

