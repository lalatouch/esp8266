#include <Arduino.h>
#include <functional>

#include "utils.h"

#define MAX_TIMEOUTS 16

namespace utils {

// Array containing all the timeouts
std::pair<int, CallbackFn> timeouts[MAX_TIMEOUTS];

void setup() {
	// Cleanup all timeout pairs
	for (int i = 0; i < MAX_TIMEOUTS; i++) timeouts[i].first = 0;
}

void handle() {
}

/**
 * Used to call a function at a later time
 * @param f    the function to call
 * @param ms   in how many miliseconds should we call the function
 * @return     true if successful, else false
 */
bool setTimeout(CallbackFn f, int ms) {
	int now = millis();

	if (ms <= 0) return false;

	for (int i = 0; i < MAX_TIMEOUTS; i++)
		// If we find an available slot in the timeouts
		if (timeouts[i].first == 0) {

			// Store function
			timeouts[i].first = now + ms;
			timeouts[i].second = [&f, i]() {
				// Call callback
				f();
				// Cleanup
				timeouts[i].first = 0;
			};
			return true;
		}

	// Nothing found :(
	return false;
}

}

