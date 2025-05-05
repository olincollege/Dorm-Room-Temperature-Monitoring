#include <math.h>
#include <microhttpd.h>
#include <pthread.h>
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

// Background thread to read sensor data and insert into database
void* sensor_logger_thread(void* arg) {
  float temp = NAN;
  float humidity = NAN;

  while (1) {
    if (get_measurement(SHT40_ADDR, &temp, &humidity) != 0) {
      if (fprintf(stderr, "Sensor read failed\n") < 0) {
        perror("Error writing to stderr");
      }
      continue;
    }

    if (insert_sensor_reading("sensor.db", temp, humidity, "Sensor01") != 0) {
      if (fprintf(stderr, "DB insert failed\n") < 0) {
        perror("Error writing to stderr");
      }
      continue;
    }

    printf("Recorded: %.2f°C, %.2f%%\n", temp, humidity);
    struct timespec delay = {1, 0};  // 1 second
    nanosleep(&delay, NULL);
  }

  return NULL;
}

int main(void) {
  if (create_database("sensor.db") != 0) {
    if (fprintf(stderr, "Failed to create database\n") < 0) {
      perror("Error writing to stderr");
    }
    return 1;
  }

  // Start background thread for logging
  pthread_t sensor_thread = 0;
  if (pthread_create(&sensor_thread, NULL, sensor_logger_thread, NULL) != 0) {
    perror("Failed to start sensor thread");
    return 1;
  }

  // Initialize the database connection
  init_database("sensor.db");

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
  pthread_cancel(sensor_thread);
  pthread_join(sensor_thread, NULL);
  return 0;
}
