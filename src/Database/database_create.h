/**
 * A library for creating the SQLite database schema for sensor readings.
 */
#pragma once

#include <sqlite3.h>

/**
 * Creates the `sensor_data.db` SQLite database and initializes the `sensor_readings` table.
 *
 * Opens (or creates) `sensor_data.db` and executes a SQL `CREATE TABLE` statement to ensure
 * that the `sensor_readings` table exists. Closes the database connection after the operation.
 *
 * @return 0 on success,
 *         1 if the database could not be opened,
 *         2 if the table creation failed.
 */
int create_database(const char *db_filename);
