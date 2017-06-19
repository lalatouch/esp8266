#include <ESP8266WiFi.h>

#include "led.h"
#include "wifi.h"
#include "i2c.h"
#include "http.h"
#include "utils.h"

int ledstate = LOW;

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
	http::setup();
	// Setup misc. utils
	utils::setup();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	utils::setTimeout([]() { led::pwm(led::RGB_R, 256); }, 1000);
	utils::setTimeout([]() { led::pwm(led::RGB_G, 256); }, 2000);
	utils::setTimeout([]() { led::pwm(led::RGB_B, 256); }, 3000);
}

void loop() {
	wifi::handleOTA();
	http::handle();
	utils::handle();
}

