#include <Arduino.h>
#include <Wire.h>

namespace i2c {

void setup() {
	// GPIO 4: SDA
	// GPIO 5: SCL
	Wire.begin(4, 5);
}

void scan() {
	Serial.println("Scanning I2C devices...");
	for (int i = 1; i < 128; i++) {
		Wire.beginTransmission(byte(i));
		switch (Wire.endTransmission()) {
		case 0:
			Serial.print("I2C device found at address ");
			Serial.println(i, HEX);
			break;
		case 4:
			Serial.print("Unknown error at address ");
			Serial.println(i, HEX);
		}
	}
	Serial.println("Done scanning.");
}

}

