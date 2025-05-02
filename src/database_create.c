#include "database_create.h"
#include <stdio.h>
#include <sqlite3.h>

int create_database(const char *db_filename) {
    sqlite3 *db;
    char *errMsg = 0;
    
    // Open (or create) the database
    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        return 1;  // Error opening DB
    }

    // SQL statement to create the table
    const char *create_table_sql =
        "CREATE TABLE IF NOT EXISTS sensor_readings ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "temperature REAL NOT NULL, "
        "humidity REAL NOT NULL, "
        "sensor_id TINYINT"
        ");";

    // Execute the SQL statement
    if (sqlite3_exec(db, create_table_sql, 0, 0, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 2;  // Error creating table
    }

    // Close the database
    sqlite3_close(db);
    return 0;  // Success
}