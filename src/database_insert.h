/**
 * A library for interacting with an SQLite database to insert sensor readings.
 */
#pragma once

#include <sqlite3.h>

/**
 * Opens an SQLite database.
 *
 * @param db_filename The name of the SQLite database file.
 * @param db A pointer to the SQLite database handle to initialize.
 * @return 0 on success, 1 if the database could not be opened.
 */
int open_database(const char *db_filename, sqlite3 **db);

/**
 * Prepares an SQL INSERT statement for inserting sensor readings.
 *
 * @param db The SQLite database handle.
 * @param stmt A pointer to the prepared statement to initialize.
 * @return 0 on success, 2 if statement preparation failed.
 */
int prepare_insert_statement(sqlite3 *db, sqlite3_stmt **stmt);

/**
 * Binds temperature, humidity, and sensor ID values to the prepared statement.
 *
 * @param stmt The prepared SQLite statement.
 * @param temperature The temperature reading to bind.
 * @param humidity The humidity reading to bind.
 * @param sensor_id The sensor ID string to bind.
 * @return 0 on success, 3 if binding values failed.
 */
int bind_insert_values(sqlite3_stmt *stmt, float temperature, float humidity, const char *sensor_id);

/**
 * Executes the prepared INSERT statement.
 *
 * @param stmt The prepared SQLite statement.
 * @return 0 on success, 4 if executing the statement failed.
 */
int execute_insert(sqlite3_stmt *stmt);
