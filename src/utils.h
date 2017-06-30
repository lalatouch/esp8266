#ifndef _UTILS_H
#define _UTILS_H

#include <functional>

/**
 * This file contains misc utilities useful when developing
 */

namespace utils {

/**
 * Mandatory setup function
 */
void setup();
/**
 * Handling function, call it every loop cycle
 */
void handle();

typedef std::function<void(void)> CallbackFn;
/**
 * Used to call a function at a later time
 * @param f    the function to call
 * @param ms   in how many miliseconds should we call the function
 * @return     true if successful, else false
 */
bool setTimeout(CallbackFn f, int ms);

/**
 * Used to call a function at regular intervals
 * @param f      the function to call
 * @param ms     calling period
 * @param count  How many times the function should be called. Pass -1 for
 *               infinite
 * @return       Interval ID. -1 if it could not be set
 */
int setInterval(CallbackFn f, int ms, int count = -1);
/**
 * Cancels a previously set interval set by setInterval using the interval ID
 * returned by setInterval
 * @param id     Interval ID
 */
void unsetInterval(int id);

/**
 * Converts a 32-bit integer to a string
 * @param n     integer
 * @returns     hex string representing the integer
 */
String toHexString(uint32_t n);

}

#endif

