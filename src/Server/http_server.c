// http_server.c
#include "http_server.h"

#include <microhttpd.h>
#include <string.h>

#include "api_handler.h"
#include "static_handler.h"
#include "utils.h"

// Main request dispatcher for the HTTP server
enum MHD_Result http_server_answer(void* cls, struct MHD_Connection* connection,
                                   const char* url, const char* method,
                                   const char* version, const char* upload_data,
                                   size_t* upload_data_size, void** con_cls) {
  // Mark unused parameters
  (void)cls;
  (void)version;
  (void)upload_data;
  (void)upload_data_size;
  (void)con_cls;

  // Handle CORS preflight
  if (strcmp(method, "OPTIONS") == 0) {
    struct MHD_Response* resp =
        MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    add_cors_headers(resp);
    enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
    MHD_destroy_response(resp);
    return ret;
  }

  // Route to /api/sensor
  if (strcmp(method, "GET") == 0 && strcmp(url, "/api/sensor") == 0) {
    return handle_api_sensor(connection);
  }

  // Route static file requests
  if (strcmp(method, "GET") == 0) {
    return handle_static_file(url, connection);
  }

  // Fallback: 404 Not Found
  const char* msg = "404 Not Found";
  struct MHD_Response* resp = MHD_create_response_from_buffer(
      strlen(msg), (void*)msg, MHD_RESPMEM_PERSISTENT);
  add_cors_headers(resp);
  enum MHD_Result ret =
      MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, resp);
  MHD_destroy_response(resp);
  return ret;
}
