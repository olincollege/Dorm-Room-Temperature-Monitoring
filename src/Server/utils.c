// utils.c
#include "utils.h"

#include <microhttpd.h>
#include <string.h>

// Add CORS headers to the response
void add_cors_headers(struct MHD_Response* resp) {
  MHD_add_response_header(resp, "Access-Control-Allow-Origin", "*");
  MHD_add_response_header(resp, "Access-Control-Allow-Methods",
                          "GET, POST, OPTIONS");
  MHD_add_response_header(resp, "Access-Control-Allow-Headers", "Content-Type");
}

// Infer MIME type based on file extension
const char* infer_mime_type(const char* filepath) {
  if (strstr(filepath, ".html")) {
    return "text/html";
  }
  if (strstr(filepath, ".css")) {
    return "text/css";
  }
  if (strstr(filepath, ".js")) {
    return "application/javascript";
  }
  if (strstr(filepath, ".ico")) {
    return "image/x-icon";
  }
  return "text/plain";
}
