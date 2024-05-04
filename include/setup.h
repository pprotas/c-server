#ifndef SETUP_H
#define SETUP_H

#include <netinet/in.h>

/* Sets up a socket on port 8080.
 * Returns the file descriptor for the socket, or -1 for errors. It also sets up
 * a clean ADDRESS for further usage.   */
int setup(struct sockaddr_in *address);

#endif
