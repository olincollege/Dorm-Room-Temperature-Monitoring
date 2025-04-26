#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "sensor.h"

// Define the port on which the raw TCP server will listen.
#define TCP_PORT 12345

/**
 * @brief Handles a single connected TCP client.
 *
 * This function formats the latest sensor data as JSON and sends it over
 * the raw TCP connection. It is intended to be called for each accepted client.
 *
 * @param client_fd The file descriptor representing the TCP connection.
 */
void handle_client(int client_fd);

/**
 * @brief Optionally, you can declare a starter function for the TCP server.
 *
 * This helper can encapsulate creating a socket, binding, listening, and
 * accepting connections in one function.
 *
 * @param port The port number to listen on.
 * @return int Returns 0 on success or a negative value on error.
 */
int start_tcp_server(int port);

#endif  // TCP_SERVER_H
