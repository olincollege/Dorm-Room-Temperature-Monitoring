#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DB_PATH "sensor_data.db"
#define SENSOR_ID "sensor1"
#define INTERVAL_SECONDS 2  // how often to insert

int insert_sensor_reading(float temperature, float humidity,
                          const char* sensor_id) {
  sqlite3* db;
  sqlite3_stmt* stmt;
  const char* sql =
      "INSERT INTO sensor_readings (temperature, humidity, sensor_id) VALUES "
      "(?, ?, ?);";

  if (sqlite3_open(DB_PATH, &db) != SQLITE_OK) {
    fprintf(stderr, "Failed to open DB: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare SQL: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  sqlite3_bind_double(stmt, 1, temperature);
  sqlite3_bind_double(stmt, 2, humidity);
  sqlite3_bind_text(stmt, 3, sensor_id, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "Failed to insert: %s\n", sqlite3_errmsg(db));
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;
}

int main() {
  srand(time(NULL));

  while (1) {
    float temp = 20.0f + (rand() % 1000) / 100.0f;  // 20.0 – 30.0
    float hum = 40.0f + (rand() % 3000) / 100.0f;   // 40.0 – 70.0

    if (insert_sensor_reading(temp, hum, SENSOR_ID) == 0) {
      printf("Inserted: temp=%.2f, hum=%.2f\n", temp, hum);
    } else {
      fprintf(stderr, "Failed to insert reading\n");
    }

    sleep(INTERVAL_SECONDS);
  }

  return 0;
}
