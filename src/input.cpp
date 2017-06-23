#include <Arduino.h>

#include "input.h"
#include "utils.h"

#define INPUT_PIN 16

namespace input {

// Flag to poll outside of the ISR
volatile bool triggered = false;
bool waiting = false;

void setup() {
	// Set as input, pullpu
	pinMode(INPUT_PIN, INPUT_PULLUP);

	// Interrupt handler
	attachInterrupt(digitalPinToInterrupt(INPUT_PIN), []() { triggered = true; }, CHANGE);
}

void handle() {
	// Check if triggered
	if (triggered && !waiting) {
		bool value = digitalRead(INPUT_PIN);
		waiting = true;

		// Soft debounce
		utils::setTimeout([&value]() {
			if (value == digitalRead(INPUT_PIN)) // WOHOO
				Serial.println(value ? "Toggled to high" : "Toggled to low");
			waiting = false;
		}, 100);

		triggered = false;
	}
}

}

