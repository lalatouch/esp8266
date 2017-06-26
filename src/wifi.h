#ifndef _WIFI_H
#define _WIFI_H

#include <IPAddress.h>

namespace wifi {

// Base station info
typedef struct BaseStation {
	IPAddress address;
	uint16_t port;
} BaseStation;

extern BaseStation base;

/**
 * - Sets up the wifi stack and the captive portal thing
 * - Setus up OTA programming
 */
void setup();

/**
 * Handles OTA programming
 */
void handleOTA();

/**
 * Sends data to the base station through UDP
 */
void send(const uint8_t *buffer, const size_t size);

template <typename T>
void send(const T &data) {
	send((uint8_t*)&data, (size_t)sizeof(data));
}

}

#endif

