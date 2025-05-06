#include <criterion/criterion.h>
#include <microhttpd.h>
#include <sqlite3.h>

#include "../src/Server/api_handler.h"
#include "../src/Server/db_handle.h"

// Provide a fake DB
static sqlite3* test_db = NULL;

// Override weak symbol to inject our test DB
sqlite3* get_database_handle(void) { return test_db; }

Test(api_handler, returns_valid_response) {
  // Setup in-memory DB
  cr_assert(sqlite3_open(":memory:", &test_db) == SQLITE_OK);

  const char* setup =
      "CREATE TABLE sensor_readings (timestamp INTEGER, temperature REAL, "
      "humidity REAL);"
      "INSERT INTO sensor_readings VALUES (1650000000, 23.5, 45.7);";
  char* err = NULL;
  cr_assert(sqlite3_exec(test_db, setup, NULL, NULL, &err) == SQLITE_OK);

  // Call the function under test
  enum MHD_Result result = handle_api_sensor(NULL);
  cr_assert_eq(result, MHD_YES);

  sqlite3_close(test_db);
  test_db = NULL;
}
