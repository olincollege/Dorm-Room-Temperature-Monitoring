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

void add_cors_headers(struct MHD_Response* resp) {
  MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
  MHD_add_response_header(resp, "Access-Control-Allow-Methods",
                          "GET, POST, OPTIONS");
  MHD_add_response_header(resp, "Access-Control-Allow-Headers", "Content-Type");
}

enum MHD_Result answer(void* cls, struct MHD_Connection* connection,
                       const char* url, const char* method, const char* version,
                       const char* upload_data, size_t* upload_data_size,
                       void** con_cls) {
  (void)cls;
  (void)version;
  (void)upload_data;
  (void)upload_data_size;
  (void)con_cls;

  // ✅ Handle OPTIONS (CORS preflight)
  if (strcmp(method, "OPTIONS") == 0) {
    struct MHD_Response* resp =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    add_cors_headers(resp);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // ✅ GET /api/sensor
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
      add_cors_headers(resp);
      int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
      MHD_destroy_response(resp);
      return ret;
    }

    sqlite3_finalize(stmt);
    struct MHD_Response* no_data =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    add_cors_headers(no_data);
    int ret = MHD_queue_response(connection, MHD_HTTP_NO_CONTENT, no_data);
    MHD_destroy_response(no_data);
    return ret;

  sql_error:;
    const char* err = "Internal Server Error";
    struct MHD_Response* resp = MHD_create_response_from_buffer(
        strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
    add_cors_headers(resp);
    ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // ✅ GET static files
  if (strcmp(method, "GET") == 0) {
    char filepath[256];
    if (strcmp(url, "/") == 0) {
      snprintf(filepath, sizeof(filepath), "www/index.html");
    } else {
      snprintf(filepath, sizeof(filepath), "www%s", url);
    }

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) return MHD_NO;

    struct stat sb;
    fstat(fd, &sb);

    const char* content_type = "text/plain";
    if (strstr(filepath, ".html"))
      content_type = "text/html";
    else if (strstr(filepath, ".css"))
      content_type = "text/css";
    else if (strstr(filepath, ".js"))
      content_type = "application/javascript";
    else if (strstr(filepath, ".ico"))
      content_type = "image/x-icon";

    struct MHD_Response* resp = MHD_create_response_from_fd(sb.st_size, fd);
    MHD_add_response_header(resp, "Content-Type", content_type);
    add_cors_headers(resp);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // ✅ Fallback: 404
  const char* msg = "404 Not Found";
  struct MHD_Response* resp = MHD_create_response_from_buffer(
      strlen(msg), (void*)msg, MHD_RESPMEM_PERSISTENT);
  add_cors_headers(resp);
  int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, resp);
  MHD_destroy_response(resp);
  return ret;
}
