#include <stdio.h>
#include <sqlite3.h>

int insert_sensor_reading(float temperature, float humidity, const char* sensor_id) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO sensor_readings (temperature, humidity, sensor_id) VALUES (?, ?, ?);";

    if (sqlite3_open("sensor_data.db", &db) != SQLITE_OK) {
        return 1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    // Bind values to the SQL statement
    sqlite3_bind_double(stmt, 1, temperature);
    sqlite3_bind_double(stmt, 2, humidity);
    sqlite3_bind_text(stmt, 3, sensor_id, -1, SQLITE_STATIC);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int main() {
    // Mock sensor values (replace with real reads from your sensor)
    float temperature = 23.4;
    float humidity = 55.2;
    const char *sensor_id = "sensor_01";

    insert_sensor_reading(temperature, humidity, sensor_id);

    return 0;
}
