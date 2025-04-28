#include <stdio.h>
#include "database_insert.h"
#include <sqlite3.h>

int insert_sensor_reading(const char *db_filename, float temperature, float humidity, const char *sensor_id) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO sensor_readings (temperature, humidity, sensor_id) VALUES (?, ?, ?);";

    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        return 1; // Error opening database
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 2; // Error preparing statement
    }

    // Bind values to the SQL statement
    sqlite3_bind_double(stmt, 1, temperature);
    sqlite3_bind_double(stmt, 2, humidity);
    sqlite3_bind_text(stmt, 3, sensor_id, -1, SQLITE_STATIC);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 3; // Error executing insert
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0; // Success
}
