#include <arpa/inet.h>  // For inet_ntoa()
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>  // For close()

#define PORT 12345  // TCP port to listen on
#define BACKLOG 10  // How many pending connections queue will hold

// Dummy sensor data structure (in your full system, this would be updated by a
// sensor thread)
typedef struct {
  time_t ts;
  double temperature;
  double humidity;
} sample_t;

// Global sensor sample (for demo purposes, we use static dummy data)
sample_t latest_sample = {0, 22.50, 55.00};

// Function to handle an individual client connection
void handle_client(int client_fd) {
  // Update the timestamp for demonstration purposes.
  latest_sample.ts = time(NULL);

  // Format the sample data into a JSON string. In a production system, you'd
  // use proper synchronization (e.g., mutexes) when reading the global sensor
  // sample.
  char json_buffer[256];
  snprintf(json_buffer, sizeof(json_buffer),
           "{\"timestamp\": %ld, \"temperature\": %.2f, \"humidity\": %.2f}\n",
           latest_sample.ts, latest_sample.temperature, latest_sample.humidity);

  // Send the JSON string to the connected client
  ssize_t sent = send(client_fd, json_buffer, strlen(json_buffer), 0);
  if (sent == -1) {
    perror("send");
  }

  // Close the connection after sending the data.
  close(client_fd);
}

int main(void) {
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t sin_size;

  // 1. Create a TCP socket.
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 2. Allow the socket to reuse the address (optional but helps in rapid
  // restarts).
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ==
      -1) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 3. Bind the socket to the port.
  server_addr.sin_family = AF_INET;
  server_addr.sin_port =
      htons(PORT);  // Convert port number to network byte order.
  server_addr.sin_addr.s_addr = INADDR_ANY;  // Bind to all network interfaces.
  memset(&(server_addr.sin_zero), 0, 8);     // Zero the rest of the structure.

  if (bind(server_fd, (struct sockaddr*)&server_addr,
           sizeof(struct sockaddr)) == -1) {
    perror("bind");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 4. Listen for incoming connections.
  if (listen(server_fd, BACKLOG) == -1) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Raw TCP server listening on port %d\n", PORT);

  // 5. Main server loop: accept and handle incoming client connections.
  while (1) {
    sin_size = sizeof(struct sockaddr_in);
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr,
                            &sin_size)) == -1) {
      perror("accept");
      continue;
    }

    printf("Got connection from %s\n", inet_ntoa(client_addr.sin_addr));

    // For a basic server, we handle the connection immediately.
    // For multiple concurrent clients, consider spawning a new thread or
    // process.
    handle_client(client_fd);
  }

  // 6. Clean up (this point is never reached in this demo because the loop is
  // infinite)
  close(server_fd);
  return 0;
}
