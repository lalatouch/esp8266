#include <Arduino.h>

#include "led.h"

namespace led {

void setup() {
	// ESP8266 integrated blue LED
	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);

	for (int i = 12; i <= 14; i++) {
		pinMode(i, OUTPUT);
		analogWrite(i, 0);
	}
}

}

