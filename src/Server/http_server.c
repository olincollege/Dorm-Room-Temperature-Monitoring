#include "http_server.h"

#include <fcntl.h>
#include <microhttpd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "db_handle.h"  // for `db`

#define JSON_BUF_SIZE 128

enum MHD_Result answer(void* cls, struct MHD_Connection* connection,
                       const char* url, const char* method, const char* version,
                       const char* upload_data, size_t* upload_data_size,
                       void** con_cls) {
  (void)cls;               // Mark unused parameter to avoid warnings
  (void)upload_data_size;  // Mark unused parameter to avoid warnings
  (void)con_cls;           // Mark unused parameter to avoid warnings

  // GET /
  if (strcmp(method, "GET") == 0 && strcmp(url, "/") == 0) {
    const char* path = "www/base_index.html";
    int file_descriptor = open(path, O_RDONLY);
    if (file_descriptor == -1) {
      return MHD_NO;
    }
    struct stat file_stat;
    fstat(file_descriptor, &file_stat);
    struct MHD_Response* resp =
        MHD_create_response_from_fd(file_stat.st_size, file_descriptor);
    MHD_add_response_header(resp, "Content-Type", "text/html");
    MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // GET /api/sensor
  if (strcmp(method, "GET") == 0 && strcmp(url, "/api/sensor") == 0) {
    sqlite3_stmt* stmt = NULL;
    const char* sql =
        "SELECT timestamp, temperature, humidity "
        "FROM sensor_readings ORDER BY timestamp DESC LIMIT 1;";
    if (sqlite3_prepare_v2(global_database_handle, sql, -1, &stmt, NULL) !=
        SQLITE_OK) {
      goto sql_error;
    }

    int result_code = sqlite3_step(stmt);
    if (result_code == SQLITE_ROW) {
      long timestamp = sqlite3_column_int64(stmt, 0);
      double temp = sqlite3_column_double(stmt, 1);
      double hum = sqlite3_column_double(stmt, 2);
      char json[JSON_BUF_SIZE];
      int len =
          snprintf(json, sizeof(json),
                   "{\"timestamp\":%ld,\"temperature\":%.2f,\"humidity\":%.2f}",
                   timestamp, temp, hum);
      sqlite3_finalize(stmt);

      struct MHD_Response* resp =
          MHD_create_response_from_buffer(len, json, MHD_RESPMEM_MUST_COPY);
      MHD_add_response_header(resp, "Content-Type", "application/json");
      MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
      int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
      MHD_destroy_response(resp);
      return ret;
    }

    sqlite3_finalize(stmt);
    struct MHD_Response* no_data =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_NO_CONTENT, no_data);
    MHD_destroy_response(no_data);
    return ret;

  sql_error:;
    const char* err = NULL;
    err = "Internal Server Error";
    struct MHD_Response* resp = MHD_create_response_from_buffer(
        strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // Fallback: 404
  const char* msg = "404 Not Found";
  struct MHD_Response* resp = MHD_create_response_from_buffer(
      strlen(msg), (void*)msg, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, resp);
  MHD_destroy_response(resp);
  return ret;
}
