#include <Arduino.h>

#include "led.h"

namespace led {

void setup() {
	// ESP8266 integrated blue LED
	pinMode(ONBOARD, OUTPUT);
	digitalWrite(2, HIGH);

	// RGB LED
	for (int i = 12; i <= 14; i++) {
		pinMode(i, OUTPUT);
		analogWrite(i, 0);
	}
}

void on(type led) {
	digitalWrite(led, led == ONBOARD ? LOW : HIGH);
}
void off(type led) {
	digitalWrite(led, led == ONBOARD ? HIGH : LOW);
}
void toggle(type led) {
	digitalWrite(led, digitalRead(led));
}
void pwm(type led, int value) {
	analogWrite(led, value);
}

}

