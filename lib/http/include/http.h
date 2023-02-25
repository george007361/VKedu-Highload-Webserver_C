#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define HTTP_GET "GET"
#define HTTP_HEAD "HEAD"

#define HTTP_ERROR -1
#define HTTP_SUCCESS 0

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

#define HTTP_REQUEST_MAX_LEN_BYTES 128

typedef struct request {
  char type[8];
  char uri[128];
} request;

void http_handler(int *client_socket);

#endif  // HTTP_SERVER_H_