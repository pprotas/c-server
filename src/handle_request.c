#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int handle_request(int socket_fd, struct sockaddr_in *address, char *buffer) {
  // Remember that address is just filled with 0s. This function actually
  // accepts a connection, and if it's accepted it will fill address with the
  // information of the connecting party.
  // It will also update addrlen to the actual length of the address
  // So in a way, address and addrlen are both input and output
  // accept is blocking, it will block the thread until there is a connection
  // to the socket
  int addrlen = sizeof(*address);
  int new_socket =
      accept(socket_fd, (struct sockaddr *)address, (socklen_t *)&addrlen);
  if (new_socket < 0) {
    perror("accept failed");
    return -1;
  }

  // Read up to 1024 bytes from the opened socket file descriptor and put it
  // in the buffer We don't want to overflow the buffer, so we read only up to
  // 1024 bytes, which is the buffer's size
  int bytes_read = read(new_socket, buffer, 1024);

  if (bytes_read == 0) {
    perror("connection closed");
    return -1;
  }
  if (bytes_read == -1) {
    perror("read failed");
    return -1;
  }
  return new_socket;
}
