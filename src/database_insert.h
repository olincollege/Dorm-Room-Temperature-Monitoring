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
 * @param temperature The temperature reading to insert.
 * @param humidity The humidity reading to insert.
 * @param sensor_id A string identifying the sensor that generated the reading.
 * @return 0 on success,
 *         1 if the database could not be opened,
 *         2 if the statement preparation failed,
 *         3 if the insert execution failed.
 */
int insert_sensor_reading(const char *db_filename, float temperature, float humidity, const char *sensor_id);
