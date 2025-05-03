#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Global DB handle (extern in header, defined in db.c)
extern sqlite3* global_database_handle;

void init_database(const char* path);
void close_database(void);

#endif
