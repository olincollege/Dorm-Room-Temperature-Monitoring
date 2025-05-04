// static_handler.h
#pragma once

#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include <microhttpd.h>

/**
 * Serves static files (HTML, CSS, JS, images) from the local filesystem
 * in response to HTTP GET requests.
 *
 * This function maps the requested URL path to a file within the "www/"
 * directory. It determines the appropriate MIME type based on the file
 * extension and constructs an HTTP response with the file's contents.
 *
 * @param url         The URL path requested by the client (e.g.,
 * "/index.html").
 * @param connection  The libmicrohttpd connection object representing the
 * client.
 *
 * @return MHD_YES if the file was successfully served, MHD_NO on error.
 */
enum MHD_Result handle_static_file(const char* url,
                                   struct MHD_Connection* connection);

#endif  // STATIC_HANDLER_H
