#ifndef HANDLE_REQUEST_H
#define HANDLE_REQUEST_H

#include <netinet/in.h>

/* Blocks the thread and awaits a request to the socket at the file descriptor
 * SOCKET_FD. The connecting party's IP information will be filled in ADDRESS.
 * Returns -1 for errors, otherwise returns the file descriptor for the new
 * socket on which the communication will take place.*/
int handle_request(int socket_fd, struct sockaddr_in *address, char *buffer);

#endif
