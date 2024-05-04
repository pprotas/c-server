#include "handle_request.h"
#include "handle_response.h"
#include "setup.h"
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

int server_fd;
int new_socket;

void cleanup(void) {
  printf("Cleaning up\n");
  close(new_socket);
  close(server_fd);
}

void handle_signal(int signal) {
  if (signal == SIGINT) {
    keep_running = 0;
    exit(SIGINT);
  }
}

int main() {
  atexit(cleanup);
  signal(SIGINT, handle_signal);

  // A struct is a way to group related data items into a single entity
  // It's like a custom data type that can be defined by the programmer
  // This particular struct sockaddr_in describes an Internet socket address
  struct sockaddr_in address;
  // Fill &address with 0s
  // This is a common practice to ensure that the structure starts from a clean
  // state
  // There could have been residual values contained in this memory
  // address
  memset(&address, 0, sizeof(address));

  server_fd = setup(&address);
  if (server_fd == -1) {
    perror("Could not setup socket");
    exit(EXIT_FAILURE);
  }

  // Create an array of chars and fill it with 0s. Its size is 1024 bytes.
  // This is a similar "cleaning" practice like we did at the start with
  // address.
  char buffer[1024] = {0};
  while (keep_running) {
    new_socket = handle_request(server_fd, &address, buffer);
    if (new_socket == -1) {
      perror("Could not handle request");
      exit(EXIT_FAILURE);
    }

    // We format the buffer into a string and print the request
    printf("Received message: %s\n", buffer);

    // Clean up the buffer after use to prevent issues with subsequent requests
    memset(buffer, 0, sizeof(buffer));

    if (handle_response(new_socket) == -1) {
      perror("Could not handle response");
      exit(EXIT_FAILURE);
    }

    // Close the socket for this specific client
    close(new_socket);

    // Clean up the client's IP address info after usage
    memset(&address, 0, sizeof(address));
  }

  return EXIT_SUCCESS;
}