#include <microhttpd.h>
#include <stdio.h>

#include "db_handle.h"
#include "http_server.h"

enum { PORT = 8080 };

int main(void) {
  init_database("sensor_data.db");

  struct MHD_Daemon* daemon =
      MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                       &http_server_answer, NULL, MHD_OPTION_END);
  if (!daemon) {
    if (fprintf(stderr, "Failed to start HTTP server\n") < 0) {
      perror("Error writing to stderr");
    }
    return 1;
  }

  printf("Server listening on port %d\n", PORT);
  printf("Press Enter to quit.\n");
  (void)getchar();

  MHD_stop_daemon(daemon);
  close_database();
  return 0;
}
