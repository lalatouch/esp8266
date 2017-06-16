#ifndef _LED_H
#define _LED_H

namespace led {

typedef enum type {
	ONBOARD = 2, RGB_R = 14, RGB_G = 12, RGB_B = 13
} type;

/**
 * Setups the LEDS
 */
void setup();

/**
 * Controls the LEDs
 */
void on(type led);
void off(type led);
void toggle(type led);
void pwm(type led, int value);

}

#endif

