#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int handle_response(int socket_fd) {
  // Our response is a pointer to the first char of this string. In C, the
  // string is null-terminated with \0, but this is added automatically in
  // this syntax.
  char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: "
                   "13\n\nHello world!\n";
  // Write the response to the connecting party
  if (write(socket_fd, response, strlen(response)) == -1) {
    perror("write failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
