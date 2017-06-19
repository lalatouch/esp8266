#include <Arduino.h>
#include <MPU9250.h>

#include "imu.h"
#include "led.h"

// Shorthand define
#define MPU_ADDR MPU9250_ADDRESS

// Interrupt pin for the MPU
#define INT_PIN 15

namespace imu {

// Our actual IMU
static MPU9250 mpu;

// Interrupt data
static volatile bool readIMU = false;

static inline uint8_t read(const uint8_t reg);
static inline void write(const uint8_t reg, const uint8_t val);

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
	attachInterrupt(digitalPinToInterrupt(INT_PIN), []() { readIMU = true; }, RISING);

	// Clear the interrupt flag
	read(INT_STATUS);
}

void handle() {
	if (readIMU) {
		// Fetch IMU data
		mpu.readAccelData(mpu.accelCount);
		mpu.readGyroData(mpu.gyroCount);

		// Fetch resolutions
		mpu.getAres();
		mpu.getGres();

		// Convert to Gs
		mpu.ax = (float)mpu.accelCount[0] * mpu.aRes - mpu.accelBias[0];
		mpu.ay = (float)mpu.accelCount[1] * mpu.aRes - mpu.accelBias[1];
		mpu.az = (float)mpu.accelCount[2] * mpu.aRes - mpu.accelBias[2];
		// Convert to rad/s
		mpu.gx = ((float)mpu.gyroCount[0] * mpu.gRes - mpu.gyroBias[0]) * DEG_TO_RAD;
		mpu.gy = ((float)mpu.gyroCount[1] * mpu.gRes - mpu.gyroBias[1]) * DEG_TO_RAD;
		mpu.gz = ((float)mpu.gyroCount[2] * mpu.gRes - mpu.gyroBias[2]) * DEG_TO_RAD;

		// Compute norms
		float acc = mpu.ax * mpu.ax + mpu.ay * mpu.ay + mpu.az * mpu.az,
		      gyro = mpu.gx * mpu.gx + mpu.gy * mpu.gy + mpu.gz * mpu.gz;

		if (1 == 2) {
			Serial.print(mpu.ax);
			Serial.print("\t");
			Serial.print(mpu.ay);
			Serial.print("\t");
			Serial.print(mpu.az);
			Serial.print("\t");
			Serial.print(mpu.gx);
			Serial.print("\t");
			Serial.print(mpu.gy);
			Serial.print("\t");
			Serial.print(mpu.gz);
			Serial.print("\t");
			Serial.print(acc);
			Serial.print("\t");
			Serial.print(gyro);
			Serial.print("\r");
		}

		if (acc >= 5.0) led::on(led::ONBOARD);
		else            led::off(led::ONBOARD);

		if (abs(mpu.gz) >= 1.0 && abs(mpu.gz) < 4.0) led::pwm(led::RGB_R, 16);
		else if (abs(mpu.gz) >= 4.0)                 led::pwm(led::RGB_R, 256);
		else                                         led::pwm(led::RGB_R, 0);

		// Clear up MPU's interrupt flag
		read(INT_STATUS);
		readIMU = false;
	}
}

static inline uint8_t read(const uint8_t reg) {
	return mpu.readByte(MPU_ADDR, reg);
}
static inline void write(const uint8_t reg, const uint8_t val) {
	mpu.writeByte(MPU_ADDR, reg, val);
}

}

