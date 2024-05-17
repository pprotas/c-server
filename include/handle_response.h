#ifndef HANDLE_RESPONSE_H
#define HANDLE_RESPONSE_H

#include <netinet/in.h>

/* Sends some response back to the client on the socket. Returns -1 for errors,
 * otherwise 0. */
int handle_response(int socket_fd, char **body);

#endif
