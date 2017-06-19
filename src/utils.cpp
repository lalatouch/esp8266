#include <Arduino.h>
#include <functional>

#include "utils.h"

#define MAX_TIMEOUTS 16
#define MAX_INTERVALS 16

namespace utils {

// Array containing all the timeouts
std::pair<int, CallbackFn> timeouts[MAX_TIMEOUTS];

// Interval stuff
typedef struct Interval {
	CallbackFn f;    // Function to be called
	int start,       // Timestamp at which it started (ms)
	    interval,    // Milliseconds between each call
	    count,       // How many times the function shall be called, -1 for
	                 // infinite
	    current;     // Current call count

	/**
	 * Gives the next time the function is due
	 * @return timestamp for the next call
	 */
	inline int next() const {
		return start + (current + 1) * interval;
	}

	/**
	 * Handles one call to the function
	 */
	void operator()() {
		f();
		current++;

		if (count != -1 && current >= count) {
			// Mark as available
			interval = 0;
			current = 0;
		}
	}
} Interval;
// All intervals
Interval intervals[MAX_INTERVALS];

void setup() {
	// Cleanup all timeout pairs
	for (int i = 0; i < MAX_TIMEOUTS; i++) timeouts[i].first = 0;

	// Cleanup all intervals
	for (int i = 0; i < MAX_INTERVALS; i++) intervals[i].interval = 0;
}

void handle() {
	// Check all timeouts
	for (int i = 0; i < MAX_TIMEOUTS; i++)
		if (timeouts[i].first != 0 && timeouts[i].first < millis()) {
			timeouts[i].second();
			timeouts[i].first = 0;
		}

	// Check all intervals
	for (int i = 0; i < MAX_INTERVALS; i++)
		if (intervals[i].interval != 0 && intervals[i].next() < millis())
			intervals[i]();
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
			timeouts[i].first = now + ms;
			timeouts[i].second = f;
			return true;
		}

	// Nothing found :(
	return false;
}

/**
 * Used to call a function at regular intervals
 * @param f      the function to call
 * @param ms     calling period
 * @param count  How many times the function should be called. Pass -1 for
 *               infinite
 * @return       true if added successfully
 */
int setInterval(CallbackFn f, int ms, int count) {
	int now = millis();

	if (ms <= 0) return -1;

	for (int i = 0; i < MAX_INTERVALS; i++)
		// Find an available slot
		if (intervals[i].interval == 0) {
			intervals[i].f = f;
			intervals[i].start = now;
			intervals[i].interval = ms;
			intervals[i].count = count;

			return i;
		}

	return -1;
}
void unsetInterval(int id) {
	if (id < 0 || id >= MAX_INTERVALS) return;

	intervals[id].interval = 0;
	intervals[id].current = 0;
}

}

