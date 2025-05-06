#include "db_handle.h"

#include <stdio.h>
#include <stdlib.h>

static sqlite3* db_handle = NULL;  // NOLINT

sqlite3* get_database_handle(void) { return db_handle; }

void init_database(const char* path) {
  if (sqlite3_open(path, &db_handle) != SQLITE_OK) {
    (void)fprintf(stderr, "Cannot open database '%s': %s\n", path,
                  sqlite3_errmsg(db_handle));
    (void)fprintf(stderr, "Exiting due to database initialization failure.\n");
    abort();
  }
}

int get_latest_reading(SensorReading* out) {
  const char* sql =
      "SELECT timestamp, temperature, humidity "
      "FROM sensor_readings ORDER BY timestamp DESC LIMIT 1;";

  sqlite3_stmt* stmt = NULL;
  if (!db_handle ||
      sqlite3_prepare_v2(db_handle, sql, -1, &stmt, NULL) != SQLITE_OK) {
    return 1;
  }

  int result = sqlite3_step(stmt);
  if (result == SQLITE_ROW) {
    out->timestamp = sqlite3_column_int64(stmt, 0);
    out->temperature = sqlite3_column_double(stmt, 1);
    out->humidity = sqlite3_column_double(stmt, 2);
    sqlite3_finalize(stmt);
    return 0;  // Success
  }

  sqlite3_finalize(stmt);
  return 2;  // No row
}

void close_database(void) {
  if (db_handle) {
    sqlite3_close(db_handle);
  }
}
