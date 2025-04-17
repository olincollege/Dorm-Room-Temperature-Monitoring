#include <stdio.h>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    char *errMsg = 0;

    // Open a new database file (or open it if it exists)
    int rc = sqlite3_open("example.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("Opened database successfully.\n");
    }

    // SQL statement to create a table
    const char* create_table_sql =
        "CREATE TABLE IF NOT EXISTS sensor_readings ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "temperature REAL NOT NULL, "
        "humidity REAL NOT NULL, "
        "sensor_id TEXT"
        ");";

    // Execute SQL statement
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Table created successfully.\n");
    }

    // Close database
    sqlite3_close(db);
    return 0;
}
