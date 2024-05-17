#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int handle_response(int socket_fd, char **body) {
  // Our response is a pointer to the first char of this string. In C, the
  // string is null-terminated with \0, but this is added automatically in
  // this syntax.
  char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n";

  char result[128];

  strcpy(result, response);

  strcat(result, "\r\n");
  strcat(result, *body);
  strcat(result, "\r\n");

  // Write the response to the connecting party
  if (write(socket_fd, result, strlen(result)) == -1) {
    perror("write failed");
    return -1;
  }

  return 1;
}
