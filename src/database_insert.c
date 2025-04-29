#include <stdio.h>
#include "database_insert.h"
#include <sqlite3.h>

int open_database(const char *db_filename, sqlite3 **db) {
    if (sqlite3_open(db_filename, db) != SQLITE_OK) {
        return 1; // Error opening database
    }
    return 0; // Success
}

int prepare_insert_statement(sqlite3 *db, sqlite3_stmt **stmt) {
    const char *sql = "INSERT INTO sensor_readings (temperature, humidity, sensor_id) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, stmt, NULL) != SQLITE_OK) {
        return 2; // Error preparing statement
    }
    return 0; // Success
}

int bind_insert_values(sqlite3_stmt *stmt, float temperature, float humidity, const char *sensor_id) {
    if (sqlite3_bind_double(stmt, 1, temperature) != SQLITE_OK ||
        sqlite3_bind_double(stmt, 2, humidity) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, sensor_id, -1, SQLITE_STATIC) != SQLITE_OK) {
        return 3; // Error binding values
    }
    return 0; // Success
}

int execute_insert(sqlite3_stmt *stmt) {
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return 4; // Error executing statement
    }
    return 0; // Success
}

int insert_sensor_reading(const char *db_filename, float temperature, float humidity, const char *sensor_id) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int result = 0;

    result = open_database(db_filename, &db);
    if (result != 0) {
        return result;
    }

    result = prepare_insert_statement(db, &stmt);
    if (result != 0) {
        sqlite3_close(db);
        return result;
    }

    result = bind_insert_values(stmt, temperature, humidity, sensor_id);
    if (result != 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return result;
    }

    result = execute_insert(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}