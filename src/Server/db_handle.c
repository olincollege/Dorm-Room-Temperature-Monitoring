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

void close_database(void) {
  if (db_handle) {
    sqlite3_close(db_handle);
  }
}
