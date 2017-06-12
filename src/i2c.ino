#include <Wire.h>

namespace i2c {

/**
 * Scans the I2C bus and prints the addresses of the found devices to the serial
 */
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

