// utils.h
#pragma once

#include <microhttpd.h>

/**
 * Adds standard CORS headers to the given HTTP response.
 *
 * This function appends the following headers to the response:
 * - Access-Control-Allow-Origin: *
 * - Access-Control-Allow-Methods: GET, POST, OPTIONS
 * - Access-Control-Allow-Headers: Content-Type
 *
 * These headers enable cross-origin requests, allowing clients from different
 * origins to access the server's resources.
 *
 * @param resp Pointer to the libmicrohttpd response object.
 */
void add_cors_headers(struct MHD_Response* resp);

/**
 * Determines the MIME type based on a file's extension.
 *
 * This function analyzes the provided file path and returns a string
 * representing the appropriate MIME type (e.g., "text/html" for ".html" files).
 * It is used to set the "Content-Type" header in HTTP responses.
 *
 * @param filepath The file path whose MIME type is to be determined.
 * @return A string representing the MIME type. Defaults to
 * "application/octet-stream" if the file extension is unrecognized.
 */
const char* infer_mime_type(const char* filepath);
