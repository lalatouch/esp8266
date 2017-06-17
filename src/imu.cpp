#include <Arduino.h>
#include <MPU9250.h>

#include "imu.h"

// Shorthand define
#define MPU_ADDR MPU9250_ADDRESS

// Interrupt pin for the MPU
#define INT_PIN 15

namespace imu {

MPU9250 mpu;

void onMPUData();
inline uint8_t read(const uint8_t reg);
inline void write(const uint8_t reg, const uint8_t val);

/**
 * Main setup function
 */
void setup() {
	// Sanity check (aka test communication)
	if (read(WHO_AM_I_MPU9250) != 0x71)
		// Warn through the serial
		Serial.println("[WARN] Incorrect WHO_AM_I for the IMU");

	// Calibrate acc and gyro
	mpu.calibrateMPU9250(mpu.gyroBias, mpu.accelBias);

	// Configure MPU
	mpu.initMPU9250();

	// Setup interrupt pin (input, active high, push-pull)
	pinMode(INT_PIN, INPUT);
	digitalWrite(INT_PIN, LOW);
	attachInterrupt(digitalPinToInterrupt(INT_PIN), onMPUData, RISING);
}

void onMPUData() {
	Serial.print("INT_STATUS: ");
	Serial.println(read(INT_STATUS), HEX);
}

inline uint8_t read(const uint8_t reg) {
	return mpu.readByte(MPU_ADDR, reg);
}
inline void write(const uint8_t reg, const uint8_t val) {
	mpu.writeByte(MPU_ADDR, reg, val);
}

}

