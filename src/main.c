#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Server/db_handle.h"
#include "Server/http_server.h"
#include "database_create.h"
#include "database_insert.h"
#include "sensor.h"

enum { PORT = 8080 };

int main(void) {
  if (create_database("sensor.db") != 0) {
    fprintf(stderr, "Failed to create database\n");
    return 1;
  }

  float temp, humidity;
  while (1) {
    if (get_measurement(SHT40_ADDR, &temp, &humidity) != 0) {
      fprintf(stderr, "Sensor read failed\n");
      break;
    }
    if (insert_sensor_reading("sensor.db", temp, humidity, "Sensor01") != 0) {
      fprintf(stderr, "DB insert failed\n");
      break;
    }
    printf("Recorded: %.2f°C, %.2f%%\n", temp, humidity);
    sleep(1);
  }

  init_database("sensor_data.db");

  struct MHD_Daemon* daemon =
      MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &answer,
                       NULL, MHD_OPTION_END);
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
