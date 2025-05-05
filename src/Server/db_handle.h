#ifndef DB_H
#define DB_H

#include <sqlite3.h>

/**
 * Global SQLite database handle.
 *
 * This external variable represents the primary connection to the SQLite
 * database used throughout the application. It should be initialized by
 * calling `init_database()` before use and closed with `close_database()`
 * when no longer needed.
 */
// db_handle.h
sqlite3* get_database_handle(void);

/**
 * Initializes the SQLite database connection.
 *
 * Opens a connection to the SQLite database at the specified file path
 * and assigns it to `global_database_handle`. If the database file does
 * not exist, it will be created.
 *
 * @param path The file path to the SQLite database.
 */
void init_database(const char* path);

/**
 * Closes the SQLite database connection.
 *
 * Finalizes the connection represented by `global_database_handle` and
 * releases associated resources. After calling this function, the global
 * handle should not be used unless reinitialized.
 */
void close_database(void);

#endif  // DB_H
