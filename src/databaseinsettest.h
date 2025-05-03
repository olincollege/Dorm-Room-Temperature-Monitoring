/**
 * A library for inserting sensor readings into an SQLite database.
 */
#pragma once

#include <sqlite3.h>

/**
 * Insert a sensor reading into the `sensor_readings` table of `sensor_data.db`.
 *
 * Opens the SQLite database `sensor_data.db`, prepares an SQL `INSERT` statement,
 * binds the temperature, humidity, and sensor ID values to the statement, and executes it.
 * Closes the database connection after the operation.
 *
 * @param temperature The temperature reading to insert into db.
 * @param humidity The humidity reading to insert into db.
 * @param sensor_id A string identifying the sensor that generated the reading.
 * @return 0 on success, 1 on failure.
 */
int insert_sensor_reading(float temperature, float humidity, const char* sensor_id);

/**
 * Main function that inserts a sample reading into the database.
 * Meant for testing or demonstration purposes.
 *
 * @return 0 on success, 1 on failure.
 */
int main();
