// http_server.h
#pragma once

#include <microhttpd.h>

/**
 * Handles incoming HTTP requests and dispatches them to the appropriate
 * route handler.
 *
 * @param cls                 Custom user-defined context (unused).
 * @param connection          Pointer to the libmicrohttpd connection object.
 * @param url                 Requested URL path (e.g., "/api/sensor").
 * @param method              HTTP method used (e.g., "GET", "POST", "OPTIONS").
 * @param version             HTTP version string (e.g., "HTTP/1.1").
 * @param upload_data         Data uploaded via POST (unused for GET).
 * @param upload_data_size    Size of the uploaded data buffer.
 * @param con_cls             Per-connection user context (unused).
 *
 * @return MHD_YES on success, MHD_NO on failure.
 */
enum MHD_Result http_server_answer(void* cls, struct MHD_Connection* connection,
                                   const char* url, const char* method,
                                   const char* version, const char* upload_data,
                                   size_t* upload_data_size, void** con_cls);
