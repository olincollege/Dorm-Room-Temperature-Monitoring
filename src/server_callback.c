// HTTP request callback function
static int request_handler(void* cls, struct MHD_Connection* connection,
                           const char* url, const char* method,
                           const char* version, const char* upload_data,
                           size_t* upload_data_size, void** con_cls) {
  // Check for GET /api/sensor
  if (strcmp(method, "GET") == 0 && strcmp(url, "/api/sensor") == 0) {
    char json_response[256];
    sample_t s;

    // Safely copy the current sample from the global variable
    pthread_mutex_lock(&sample_mutex);
    s = latest_sample;
    pthread_mutex_unlock(&sample_mutex);

    // Format the response as JSON
    snprintf(json_response, sizeof(json_response),
             "{\"timestamp\": %ld, \"temperature\": %.2f, \"humidity\": %.2f}",
             s.ts, s.temperature, s.humidity);

    struct MHD_Response* response = MHD_create_response_from_buffer(
        strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_COPY);
    int ret = MHD_queue_response(connection, 200, response);
    MHD_destroy_response(response);
    return ret;
  }

  // For any other endpoint or method, return 404.
  const char* error_msg = "Not Found";
  struct MHD_Response* response = MHD_create_response_from_buffer(
      strlen(error_msg), (void*)error_msg, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response(connection, 404, response);
  MHD_destroy_response(response);
  return ret;
}
