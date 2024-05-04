#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // A socket is a way to communicate with a different machine
  // AF_INET is the domain of IPv4 addresses
  // SOCK_STREAM is a two-way byte stream that uses TCP
  // Protocol (0) would default to IPPROTO_TCP
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  // -1 means there was an error
  if (server_fd == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

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
  // sin_family is declared within this struct using some cool macro that
  // prefixes 'family' by 'sin_'
  // We set the address family to AF_INET, which indicates that this is an IPv4
  // address
  address.sin_family = AF_INET;
  // This means we accept connections on any of this server's IP addresses
  // We could set it to a specific IP address with inet_addr("192.168.1.100")
  address.sin_addr.s_addr = INADDR_ANY;
  // Set port number to 8080
  // htons (host byte order to network byte order short) converts our port 8080
  // from host byte order to network byte order Different architectures would
  // store bytes in different ways This ensures we always use Big Endian
  // Basically, we have to ensure that the machines that communicate over a
  // network agree on the order in which the bytes are interpreted. Big Endian
  // means storing the bytes from most significant to least, and Little Endian
  // is the other way around
  // We are the host, and we convert our byte order (could be Little or Big
  // Endian) to the standardized network byte order, which is always Big Endian
  // If we don't do this conversion, we would listen on the wrong port on some
  // machines
  address.sin_port = htons(8080);

  // Here we are binding the IP address to our socket file descriptor
  // In UNIX, file descriptors are handles used to access files and other I/O
  // resources like sockets
  // The second parameter is kind of confusing. We are getting the memory
  // address of sockaddr_in, then we are creating a pointer to this memory
  // address. But we don't want a pointer to sockaddr_in (because the bind
  // function is more general), but a sockaddr, so we have to cast it.

  // The syntax can be a bit confusing. We are getting the pointer to address
  // with &address. Then we cast it to another pointer, but of a different type
  // (sockadrr instead of sockaddr_in)
  // The & operator is only ever used to get the pointer (memory address) to
  // some variable
  // The * operator is used to dereference a pointer, and get the
  // actual value at that memory address
  // But the * operator is also used in declarations to denote that the pointer
  // points to a specific type. So in this case, "struct sockaddr *" means that
  // we are pointing to a struct sockaddr.
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Prepare to accept connections on this socket file descriptor
  // There are 10 max pending connections, after that connections will be
  // refused
  if (listen(server_fd, 10) == -1) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

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
