#ifndef _WIFI_H
#define _WIFI_H

namespace wifi {

/**
 * - Sets up the wifi stack and the captive portal thing
 * - Setus up OTA programming
 */
void setup();

/**
 * Handles OTA programming
 */
void handleOTA();

}

#endif

