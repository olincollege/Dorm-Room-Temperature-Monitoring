#include <fcntl.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define PORT 8080

// Declare the database
static sqlite3* db;

static enum MHD_Result answer(void* cls, struct MHD_Connection* connection,
                              const char* url, const char* method,
                              const char* version, const char* upload_data,
                              size_t* upload_data_size, void** con_cls) {
  if (strcmp(method, "GET") == 0 && strcmp(url, "/") == 0) {
    const char* path = "www/index.html";
    int fd = open(path, O_RDONLY);
    if (fd == -1) return MHD_NO;
    struct stat sb;
    fstat(fd, &sb);
    struct MHD_Response* resp = MHD_create_response_from_fd(sb.st_size, fd);
    MHD_add_response_header(resp, "Content-Type", "text/html");
    MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // 2) GET /api/sensor → real JSON from the DB
  if (strcmp(method, "GET") == 0 && strcmp(url, "/api/sensor") == 0) {
    sqlite3_stmt* stmt;
    const char* sql =
        "SELECT timestamp, temperature, humidity "
        "  FROM sensor_readings "
        " ORDER BY timestamp DESC "
        " LIMIT 1;";

    // 1) Prepare once
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
      fprintf(stderr, "[ERROR] sqlite3_prepare_v2: %s\n", sqlite3_errmsg(db));
      goto sql_error;
    }
    fprintf(stderr, "[DEBUG] SQL prepared OK\n");

    // 2) Step once
    rc = sqlite3_step(stmt);
    fprintf(stderr, "[DEBUG] sqlite3_step returned %d\n", rc);

    if (rc == SQLITE_ROW) {
      long ts = sqlite3_column_int64(stmt, 0);
      double temp = sqlite3_column_double(stmt, 1);
      double hum = sqlite3_column_double(stmt, 2);

      char json[128];
      int len =
          snprintf(json, sizeof(json),
                   "{\"timestamp\":%ld,\"temperature\":%.2f,\"humidity\":%.2f}",
                   ts, temp, hum);

      sqlite3_finalize(stmt);

      struct MHD_Response* resp = MHD_create_response_from_buffer(
          len, (void*)json, MHD_RESPMEM_MUST_COPY);
      MHD_add_response_header(resp, "Content-Type", "application/json");
      MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
      int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
      MHD_destroy_response(resp);
      return ret;
    }

    // No row found
    sqlite3_finalize(stmt);
    {
      struct MHD_Response* resp =
          MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
      int ret = MHD_queue_response(connection, MHD_HTTP_NO_CONTENT, resp);
      MHD_destroy_response(resp);
      return ret;
    }

  sql_error:
    // SQL error → 500
    {
      const char* err = "Internal Server Error";
      struct MHD_Response* resp = MHD_create_response_from_buffer(
          strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
      int ret =
          MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, resp);
      MHD_destroy_response(resp);
      return ret;
    }
  }

  // 3) All other requests → 404
  const char* notfound = "404 Not Found";
  struct MHD_Response* resp = MHD_create_response_from_buffer(
      strlen(notfound), (void*)notfound, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, resp);
  MHD_destroy_response(resp);
  return ret;
}

// Call this once in main(), before starting the HTTP daemon:
void init_database(const char* path) {
  if (sqlite3_open(path, &db) != SQLITE_OK) {
    fprintf(stderr, "Cannot open database '%s': %s\n", path,
            sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
  }
}

// Call this once on shutdown:
void close_database(void) { sqlite3_close(db); }

int main(void) {
  // Initialize the database
  init_database("sensor_data.db");
  fprintf(stderr, "Opened DB OK, handle=%p\n", (void*)db);
  struct MHD_Daemon* daemon =
      MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &answer,
                       NULL, MHD_OPTION_END);
  if (!daemon) {
    fprintf(stderr, "Failed to start HTTP server\n");
    return 1;
  }
  printf("Server listening on port %d\n", PORT);
  printf("Press Enter to stop.\n");
  getchar();  // wait for user
  MHD_stop_daemon(daemon);
  close_database();
  return 0;
}
