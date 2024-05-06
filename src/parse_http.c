#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_request_line(char *request_line, char **method, char **request_uri,
                       char **http_version);
int parse_header(char *header, char **key, char **value);

int parse_http(char *http) {
  // Since we will be doing a "nested" strtok in a bit, we can use strtok_r with
  // a save pointer to continue where we left off later
  char *saveptr;
  // Get the first line, which contains the request information
  char *token = strtok_r(http, "\n", &saveptr);

  // Parse the request line
  char *method, *request_uri, *http_version;
  parse_request_line(token, &method, &request_uri, &http_version);

  // Some basic header checks here. In a more realistic program, this would be
  // way more complicated to support all methods and routes.
  if (strcmp(method, "POST") != 0 || strcmp(method, "GET") != 0) {
    perror("Only GET and POST methods are supported");

    free(method);
    free(request_uri);
    free(http_version);

    return -1;
  }

  if (strcmp(request_uri, "/") != 0) {
    perror("Only the / route is supported");

    free(method);
    free(request_uri);
    free(http_version);

    return -1;
  }

  // We ignore HTTP version for now, since it makes no difference for the
  // purposes of our program

  free(request_uri);
  free(http_version);

  // Continue with the rest of the request
  token = strtok_r(NULL, "\n", &saveptr);

  char *key, *value;
  while (token != NULL && strcmp(token, "\n") != 0) {
    parse_header(token, &key, &value);
    token = strtok_r(NULL, "\n", &saveptr);
    // TODO: Do some header parsing
    // Return if Accept is not plain text
    // Return if Content-Type is not plain text
  }

  // TODO: Get message body if POST

  // TODO: Probably just return method & body to keep it simple. Make the caller
  // responsible for the memory management.

  free(key);
  free(value);

  return 0;
}

int parse_request_line(char *request_line, char **method, char **request_uri,
                       char **http_version) {
  // Each part of the request line is separated by spaces
  char *token = strtok(request_line, " ");
  *method = strdup(token);

  token = strtok(NULL, " ");
  *request_uri = strdup(token);

  token = strtok(NULL, " ");
  *http_version = strdup(token);

  return 0;
}

int parse_header(char *header, char **key, char **value) {
  char *token = strtok(header, ":");
  *key = strdup(token);

  token = strtok(header, ":");
  *value = strdup(token);

  return 0;
}
