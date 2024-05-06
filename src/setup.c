#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int setup(struct sockaddr_in *address) {
  // A socket is a way to communicate with a different machine
  // AF_INET is the domain of IPv4 addresses
  // SOCK_STREAM is a two-way byte stream that uses TCP
  // Protocol (0) would default to IPPROTO_TCP
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  // -1 means there was an error
  if (server_fd == -1) {
    perror("Socket creation failed");
    return -1;
  }

  // sin_family is declared within this struct using some cool macro that
  // prefixes 'family' by 'sin_'
  // We set the address family to AF_INET, which indicates that this is an IPv4
  // address
  address->sin_family = AF_INET;
  // This means we accept connections on any of this server's IP addresses
  // We could set it to a specific IP address with inet_addr("192.168.1.100")
  address->sin_addr.s_addr = INADDR_ANY;
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
  address->sin_port = htons(8080);

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
  // Set SO_REUSEADDR to 1 to reuse the port
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) == -1) {
    perror("Bind failed");
    return -1;
  }

  // Prepare to accept connections on this socket file descriptor
  // There are 10 max pending connections, after that connections will be
  // refused
  if (listen(server_fd, 10) == -1) {
    perror("Listen failed");
    return -1;
  }

  return server_fd;
}
