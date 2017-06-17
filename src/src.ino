#include <ESP8266WiFi.h>

#include "led.h"
#include "wifi.h"
#include "i2c.h"
#include "http.h"
#include "imu.h"

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
	// Setup IMU
	imu::setup();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println("Ax\tAy\tAz\tGx\tGy\tGz");
}

void loop() {
	wifi::handleOTA();
	http::handle();
	imu::handle();
}

