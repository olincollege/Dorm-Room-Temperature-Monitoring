/**
 * A program for initializing an SQLite database to store sensor readings.
 */
#pragma once

// Creates the database and sensor_readings table if it doesn't exist.
// Returns 0 on success, 1 on database open error, 2 on table creation error.
int create_database(const char *db_filename);
