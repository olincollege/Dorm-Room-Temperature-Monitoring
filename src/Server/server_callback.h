#ifndef SERVER_H
#define SERVER_H

#include <microhttpd.h>

#include "sensor.h"  // for sample_t

// Define the port on which the HTTP server will listen.
#define HTTP_PORT 8080

/**
 * @brief HTTP request handler for libmicrohttpd.
 *
 * This callback is called by libmicrohttpd for each incoming request. It
 * inspects the request (e.g., if the URL is "/api/sensor" and method is GET),
 * retrieves the current sensor sample (with proper mutex locking) and sends
 * back a JSON response.
 *
 * @param cls       A pointer to your application data (if needed).
 * @param connection The connection object.
 * @param url        The URL requested.
 * @param method     The HTTP method (GET, POST, etc.).
 * @param version    The HTTP version.
 * @param upload_data Data uploaded by the client (if any).
 * @param upload_data_size Size of the upload data.
 * @param con_cls    A pointer to per-connection storage.
 * @return int       An HTTP status code to be sent as response.
 */
int request_handler(void* cls, struct MHD_Connection* connection,
                    const char* url, const char* method, const char* version,
                    const char* upload_data, size_t* upload_data_size,
                    void** con_cls);

#endif  // SERVER_H
