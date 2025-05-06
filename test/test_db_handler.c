#include <criterion/criterion.h>
#include <sqlite3.h>

#include "../src/Server/db_handle.h"

Test(db_handle, reads_latest_row) {
  sqlite3* db;
  cr_assert(sqlite3_open(":memory:", &db) == SQLITE_OK);

  char* err = NULL;
  const char* setup =
      "CREATE TABLE sensor_readings (timestamp INTEGER, temperature REAL, "
      "humidity REAL);"
      "INSERT INTO sensor_readings VALUES (123456, 22.1, 44.8);";
  cr_assert(sqlite3_exec(db, setup, NULL, NULL, &err) == SQLITE_OK);

  struct Reading {
    float temperature;
    float humidity;
  } reading = {0.0, 0.0};

  int ok = get_latest_reading(&reading);
  float temp = reading.temperature;
  float hum = reading.humidity;
  cr_assert_eq(ok, 0);
  cr_expect_float_eq(temp, 22.1f, 1e-2);
  cr_expect_float_eq(hum, 44.8f, 1e-2);

  sqlite3_close(db);
}
