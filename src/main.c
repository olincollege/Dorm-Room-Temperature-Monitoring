int main(void) {
  // Initialize global sample with dummy data
  latest_sample.ts = time(NULL);
  latest_sample.temperature = 0.0;
  latest_sample.humidity = 0.0;

  // Start sensor polling thread
  pthread_t sensor_thread;
  if (pthread_create(&sensor_thread, NULL, sensor_polling_thread, NULL) != 0) {
    perror("Failed to create sensor thread");
    return 1;
  }

  // Start the HTTP server using libmicrohttpd
  struct MHD_Daemon* daemon =
      MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                       &request_handler, NULL, MHD_OPTION_END);
  if (daemon == NULL) {
    fprintf(stderr, "Failed to start HTTP server.\n");
    return 1;
  }

  printf("Server is running on port %d. Press Enter to quit...\n", PORT);
  getchar();  // Wait for user input to exit

  // Cleanup on exit
  MHD_stop_daemon(daemon);
  pthread_cancel(sensor_thread);
  pthread_join(sensor_thread, NULL);
  return 0;
}
