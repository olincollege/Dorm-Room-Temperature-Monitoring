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
    srand(time(NULL));  // Seed the random number generator

    for (int i = 0; i < 10; ++i) {
        float temperature = 15.0 + (rand() % 1500) / 100.0f;  // 15.0 to 30.0
        float humidity = 30.0 + (rand() % 7000) / 100.0f;     // 30.0 to 100.0

        char sensor_id[16];
        snprintf(sensor_id, sizeof(sensor_id), "sensor_%d", i + 1);

        insert_sensor_reading(temperature, humidity, sensor_id);
    }

    return 0;
}
