#include "setup.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
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
  int server_fd = setup(&address);
  // Remember that address is just filled with 0s. This function actually
  // accepts a connection, and if it's accepted it will will address with the
  // information of the connecting party.
  // It will also update addrlen to the actual length of the address
  // So in a way, address and addrlen are both input and output
  int addrlen = sizeof(address);
  int new_socket =
      accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  if (new_socket < 0) {
    perror("accept failed");
    exit(EXIT_FAILURE);
  }

  // Create an array of chars and fill it with 0s. It's size is 1024 bytes. This
  // is a similar "cleaning" practice like we did at the start with address.
  char buffer[1024] = {0};
  // Read up to 1024 bytes from the opened socket file descriptor and put it in
  // the buffer
  // We don't want to overflow the buffer, so we read only up to 1024 bytes,
  // which is the buffer's size
  int bytes_read = read(new_socket, buffer, 1024);

  if (bytes_read == 0) {
    perror("connection closed");
    exit(EXIT_FAILURE);
  }
  if (bytes_read == -1) {
    perror("read failed");
    exit(EXIT_FAILURE);
  }

  // We format the buffer into a string and print the request
  printf("Received message: %s\n", buffer);

  // Our response is a pointer to the first char of this string. In C, the
  // string is null-terminated with \0, but this is added automatically in this
  // syntax.
  char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: "
                   "13\n\nHello world!\n";
  // Write the response to the connecting party
  if (write(new_socket, response, strlen(response)) == -1) {
    perror("write failed");
    exit(EXIT_FAILURE);
  }

  close(new_socket);
  close(server_fd);
}
