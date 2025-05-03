#include "db_handle.h"

#include <stdio.h>
#include <stdlib.h>

sqlite3* global_database_handle = NULL;

void init_database(const char* path) {
  if (sqlite3_open(path, &global_database_handle) != SQLITE_OK) {
    fprintf(stderr, "Cannot open database '%s': %s\n", path,
            sqlite3_errmsg(global_database_handle));
    sqlite3_close(global_database_handle);
    fprintf(stderr, "Exiting due to database initialization failure.\n");
    abort();
  }
}

void close_database(void) {
  if (global_database_handle) {
    sqlite3_close(global_database_handle);
  }
}
