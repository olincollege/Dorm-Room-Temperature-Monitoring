/**
 * A library for getting sensor values from an SHT40 sensor on a Raspberry Pi.
 */
#pragma once

#include <stdlib.h>

#define I2C_DEV "/dev/i2c-1"
#define SHT40_ADDR 0x44
#define SHT40_MEASURE_HIGH_PREC 0xFD

/**
 * Open the I2C bus and select an I2C device.
 *
 * @param dev File path to the I2C library.
 * @param addr The specific adress of the SHT40 sensor we are using.
 * @return The file descriptor attached to the sensor.
 */
int open_and_connect(char* dev, int addr);

/**
 * Take a 6 byte sensor reading from an SHT40 sensor and return temp reading.
 *
 * @param bytes_read 6 byte SHT40 sensor reading.
 * @return temperature reading in celsius from sensor.
 */
float convert_temp(unsigned char bytes_read[6]);

/**
 * Take a 6 byte sensor reading from an SHT40 sensor and return humid reading.
 *
 * @param bytes_read 6 byte SHT40 sensor reading.
 * @return relative humidity reading from sensor.
 */
float convert_humidity(unsigned char bytes_read[6]);

/**
 * Get a measurement from the sensor and store it in pointers passed in.
 * 
 * Send a measurement command, wait for a response to that command, and then
 * read the response. Convert the raw data in the response to a temperature in
 * celsius and a relative humidity. Store that data in the pointers passed to
 * the function.
 *
 * @param addr The specific adress of the SHT40 sensor we are using.
 * @param temp Pointer to temperature variable.
 * @param humidity Pointer to humidity variable.
 * @return An int representing the success of gathering a measurement.
 */
int get_measurement(int addr, float* temp, float* humidity);

/**
 * Get a measurement from the sensor and store it in pointers passed in.
 * 
 * Loop measurements times and run get_measurements, printing the updated
 * temperature and humidity reading. Sleep for delay seconds between iterations
 * of the loop.
 *
 * @param addr The specific adress of the SHT40 sensor we are using.
 * @param measurements The number of measurements to gather from the sensor.
 * @param delay The time in seconds to wait between each reading.
 * @return An int representing the success of gathering a measurement.
 */
int print_measurements(int addr, size_t measurements, size_t delay);


