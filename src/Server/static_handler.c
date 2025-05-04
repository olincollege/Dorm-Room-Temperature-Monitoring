// static_handler.c
#include "static_handler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

#define FILEPATH_BUFFER_SIZE 256

// Handles GET requests for static files like HTML, JS, CSS
enum MHD_Result handle_static_file(const char* url,
                                   struct MHD_Connection* connection) {
  char filepath[FILEPATH_BUFFER_SIZE];
  if (strcmp(url, "/") == 0) {
    snprintf(filepath, sizeof(filepath), "www/index.html");
  } else {
    snprintf(filepath, sizeof(filepath), "www%s", url);
  }

  int file_descriptor = open(filepath, O_RDONLY);
  if (file_descriptor == -1) {
    return MHD_NO;
  }

  struct stat file_stat;
  fstat(file_descriptor, &file_stat);

  const char* content_type = infer_mime_type(filepath);
  struct MHD_Response* resp =
      MHD_create_response_from_fd(file_stat.st_size, file_descriptor);
  MHD_add_response_header(resp, "Content-Type", content_type);
  add_cors_headers(resp);

  int ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
  MHD_destroy_response(resp);
  return ret;
}
