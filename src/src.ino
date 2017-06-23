#include <ESP8266WiFi.h>

#include "led.h"
#include "wifi.h"
#include "i2c.h"
#include "http.h"
#include "imu.h"
#include "utils.h"
#include "input.h"

int ledstate = LOW;

void setup() {
	// Init serial (debugging)
	Serial.begin(115200);
	Serial.println("Booting");

	// Setup misc. utils
	utils::setup();
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
	// Setup the play/pause "sensor"
	input::setup();

	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println("Ax\tAy\tAz\tGx\tGy\tGz");

	utils::setTimeout([]() { led::pwm(led::RGB_R, 256); }, 1000);
	utils::setTimeout([]() { led::pwm(led::RGB_G, 256); }, 2000);
	utils::setTimeout([]() { led::pwm(led::RGB_B, 256); }, 3000);
	utils::setTimeout([]() {
		led::pwm(led::RGB_R, 0);
		led::pwm(led::RGB_G, 0);
		led::pwm(led::RGB_B, 0);
	}, 4000);

	utils::setInterval([]() {
		ledstate = ledstate == LOW ? HIGH : LOW;
		if (ledstate == LOW) led::on(led::ONBOARD);
		else                 led::off(led::ONBOARD);
	}, 500, 10);
}

void loop() {
	wifi::handleOTA();
	http::handle();
	imu::handle();
	utils::handle();
	input::handle();
}

