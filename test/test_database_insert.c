#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <sqlite3.h>
#include "../src/Database/database_insert.h"

#define TEST_DB "test_sensor.db"

Test(open_database, valid_file) {
    sqlite3 *db = NULL;
    int result = open_database(TEST_DB, &db);
    cr_assert(eq(int, result, 0), "Expected success opening valid DB");
    sqlite3_close(db);
}

Test(open_database, invalid_file) {
    sqlite3 *db = NULL;
    int result = open_database("/invalid/path/fake.db", &db);
    cr_assert(eq(int, result, 1), "Expected failure for invalid DB path");
}

Test(prepare_insert_statement, valid_schema) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    open_database(TEST_DB, &db);
    int result = prepare_insert_statement(db, &stmt);
    cr_assert(eq(int, result, 0), "Expected successful SQL statement preparation");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

Test(bind_insert_values, binds_correctly) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    open_database(TEST_DB, &db);
    prepare_insert_statement(db, &stmt);

    int result = bind_insert_values(stmt, 24.3f, 55.7f, "sensor1");
    cr_assert(eq(int, result, 0), "Expected successful value binding");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

Test(execute_insert, insert_success) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    open_database(TEST_DB, &db);
    prepare_insert_statement(db, &stmt);
    bind_insert_values(stmt, 22.1f, 48.9f, "sensor2");

    int result = execute_insert(stmt);
    cr_assert(eq(int, result, 0), "Expected successful execution of insert");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
