#ifndef _I2C_H
#define _I2C_H

namespace i2c {

/**
 * Sets up I2C
 */
void setup();

/**
 * Scans the I2C bus and prints the addresses of the found devices to the serial
 */
void scan();

}

#endif

