// static_handler.c
#include "static_handler.h"

#include <fcntl.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"

enum { FILEPATH_BUFFER_SIZE = 256 };

// Handles GET requests for static files like HTML, JS, CSS
enum MHD_Result handle_static_file(const char* url,
                                   struct MHD_Connection* connection) {
  char filepath[FILEPATH_BUFFER_SIZE];
  if (strcmp(url, "/") == 0) {
    if (snprintf(filepath, sizeof(filepath), "www/index.html") >=
        (int)sizeof(filepath)) {
      if (fprintf(stderr, "Error: Filepath buffer size exceeded\n") < 0) {
        perror("fprintf failed");
      }
      return MHD_NO;
    }
  } else {
    if (snprintf(filepath, sizeof(filepath), "www%s", url) >=
        (int)sizeof(filepath)) {
      if (fprintf(stderr, "Error: Filepath buffer size exceeded\n") < 0) {
        perror("fprintf failed");
      }
      return MHD_NO;
    }
  }

  int file_descriptor = open(filepath, O_RDONLY | O_CLOEXEC);

  if (file_descriptor == -1) {
    const char* not_found = "404 Not Found";
    struct MHD_Response* not_found_resp = MHD_create_response_from_buffer(
        strlen(not_found), (void*)not_found, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(not_found_resp, "Content-Type", "text/plain");
    add_cors_headers(not_found_resp);
    int ret =
        MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, not_found_resp);
    MHD_destroy_response(not_found_resp);
    return ret;
  }

  struct stat file_stat;
  fstat(file_descriptor, &file_stat);

  const char* content_type = infer_mime_type(filepath);
  struct MHD_Response* resp = NULL;
  if (file_stat.st_size < 0) {
    close(file_descriptor);
    return MHD_NO;
  }

  resp =
      MHD_create_response_from_fd((size_t)file_stat.st_size, file_descriptor);
  if (!resp) {
    close(file_descriptor);
    return MHD_NO;
  }
  MHD_add_response_header(resp, "Content-Type", content_type);
  add_cors_headers(resp);

  enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, resp);
  MHD_destroy_response(resp);
  return ret;
}
