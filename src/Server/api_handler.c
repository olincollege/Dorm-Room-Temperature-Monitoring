// api_handler.c
#include "api_handler.h"

#include <microhttpd.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "db_handle.h"
#include "utils.h"

enum { HTTP_STATUS_OK = 200 };

enum { JSON_BUF_SIZE = 128 };

// Handles the GET /api/sensor endpoint
enum MHD_Result handle_api_sensor(struct MHD_Connection* connection) {
  sqlite3* database_handle = get_database_handle();
  if (!database_handle) {
    if (fprintf(stderr, "[ERROR] Database handle is NULL\n") < 0) {
      perror("[ERROR] Failed to write to stderr");
    }
    return MHD_NO;
  }

  sqlite3_stmt* stmt = NULL;
  const char* sql =
      "SELECT timestamp, temperature, humidity "
      "FROM sensor_readings ORDER BY timestamp DESC LIMIT 1;";

  int prep_status = sqlite3_prepare_v2(database_handle, sql, -1, &stmt, NULL);
  if (prep_status != SQLITE_OK) {
    if (fprintf(stderr, "[ERROR] Failed to prepare SQL: %s\n",
                sqlite3_errmsg(database_handle)) < 0) {
      perror("[ERROR] Failed to write to stderr");
    }
    const char* err = "Internal Server Error";
    struct MHD_Response* resp = MHD_create_response_from_buffer(
        strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
    add_cors_headers(resp);
    int ret =
        MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, resp);
    MHD_destroy_response(resp);
    return (enum MHD_Result)ret;
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
    int ret = MHD_queue_response(connection, HTTP_STATUS_OK, resp);
    MHD_destroy_response(resp);
    return (enum MHD_Result)ret;
  }
  (void)fprintf(stderr, "[WARN] No sensor data returned (step code: %d)\n",
                result_code);
  (void)fprintf(stderr, "[WARN] SQL Error: %s\n",
                sqlite3_errmsg(database_handle));

  sqlite3_finalize(stmt);
  struct MHD_Response* no_data =
      MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
  add_cors_headers(no_data);
  enum MHD_Result ret =
      MHD_queue_response(connection, MHD_HTTP_NO_CONTENT, no_data);
  MHD_destroy_response(no_data);
  return ret;
}
