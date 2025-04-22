/**
 * A program for initializing an SQLite database to store sensor readings.
 */
#pragma once

#include <sqlite3.h>

/**
 * Main function that creates an SQLite database and a table for sensor data.
 *
 * Opens (or creates) a database named "sensor_data.db" and creates a table
 * called "sensor_readings" if it does not already exist. The table includes:
 * - id: Auto-incremented primary key
 * - timestamp: Automatically set to current time
 * - temperature: Float value, required
 * - humidity: Float value, required
 * - sensor_id: Optional text field
 *
 * @return 0 on success, 1 on failure
 */
int main();
