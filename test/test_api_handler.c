// test_api_handler.c
#include <criterion/assert.h>  // For cr_assert, cr_assert_eq, etc.
#include <criterion/criterion.h>
#include <criterion/internal/test.h>  // For Test macro
#include <sqlite3.h>

#include "api_handler.h"
#include "db_handle.h"

static sqlite3* test_db = NULL;

// Override the real get_database_handle
sqlite3* get_database_handle(void) { return test_db; }

Test(api_handler, returns_latest_sensor_data_from_test_db) {
  // Setup in-memory SQLite DB
  cr_assert(sqlite3_open(":memory:", &test_db) == SQLITE_OK);

  const char* create_sql =
      "CREATE TABLE sensor_readings (timestamp INTEGER, temperature REAL, "
      "humidity REAL);";
  const char* insert_sql =
      "INSERT INTO sensor_readings VALUES (1650000000, 22.5, 55.3);";

  char* err_msg = NULL;
  cr_assert(sqlite3_exec(test_db, create_sql, NULL, NULL, &err_msg) ==
            SQLITE_OK);
  cr_assert(sqlite3_exec(test_db, insert_sql, NULL, NULL, &err_msg) ==
            SQLITE_OK);

  // This assumes handle_api_sensor won't dereference connection
  enum MHD_Result res = handle_api_sensor(NULL);
  cr_assert_eq(res, MHD_YES);

  sqlite3_close(test_db);
  test_db = NULL;
}
