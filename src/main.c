#include <math.h>
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "Sensor/sensor.h"
#include "Server/db_handle.h"
#include "Server/http_server.h"
#include "database_create.h"
#include "database_insert.h"

enum { PORT = 8080 };

int main(void) {
  if (create_database("sensor.db") != 0) {
    if (fprintf(stderr, "Failed to create database\n") < 0) {
      perror("Error writing to stderr");
    }
    return 1;
  }

  float temp = NAN;
  float humidity = NAN;
  while (1) {
    if (get_measurement(SHT40_ADDR, &temp, &humidity) != 0) {
      if (fprintf(stderr, "Sensor read failed\n") < 0) {
        perror("Error writing to stderr");
      }
      break;
    }
    if (insert_sensor_reading("sensor.db", temp, humidity, "Sensor01") != 0) {
      if (fprintf(stderr, "DB insert failed\n") < 0) {
        perror("Error writing to stderr");
      }
      break;
    }
    printf("Recorded: %.2f°C, %.2f%%\n", temp, humidity);
    struct timespec timeSpec = {1, 0};  // 1 second, 0 nanoseconds
    nanosleep(&timeSpec, NULL);
  }

  // Initialize the database connection
  init_database("sensor_data.db");

  // Start the HTTP server
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

  // Stop the HTTP server
  MHD_stop_daemon(daemon);
  close_database();
  return 0;
}
