#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include <errno.h>

#define HTTP_GET "GET"
#define HTTP_HEAD "HEAD"

#define HTTP_ERROR -1
#define HTTP_SUCCESS 0

#define DEBUG

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

#define HTTP_REQUEST_MAX_LEN_BYTES 128
#define HTTP_RESPONCE_HEADERS_LEN_BYTES 256
#define HTTP_ROOT_DIR "/home/gg/Desktop/TP-3-sem/HL-Web-server-GIT"
#define HTTP_DIR_INDEX "index.html"
#define HTTP_URI_MAX_LEN_BYTES 128
#define HTTP_METHOD_MAX_LEN_BYTES 8

enum HTTP_FOPEN_RESULT{
  FOPEN_SUCCESS = 1,
  FOPEN_NOT_FOUND,
  FOPEN_FORBIDDEN,
  FOPEN_ERROR
};

extern  const int HTTP_ROOT_DIR_LEN;

#define HEADER_STATUS "HTTP/1.1 %d %s\n"
#define HEADER_SERVER "Server: %s\n"
#define HEADER_DATE "Date: %d\n"
#define HEADER_CONNECTION "Connection: %s\n"
#define HEADER_CONTENT_TYPE "Content-Type: %s\n"
#define HEADER_CONTENT_LENGTH "Content-Length: %i\n"
#define CRLF "\r\n"

#define SERVER_NAME "hl-web-server"
#define CONNECTION_CLOSE "close"


#define STATUS_OK 200
#define MESSAGE_OK "Success"

#define STATUS_METHOD_NOT_ALLOWED 405
#define MESSAGE_METHOD_NOT_ALLOWED "Method Not Allowed"


#define STATUS_FORBIDDEN 403
#define MESSAGE_FORBIDDEN "Forbidden"

#define STATUS_NOT_FOUND 404
#define MESSAGE_NOT_FOUND "Not Found"


#define STATUS_INTERNAL_ERROR 500
#define MESSAGE_INTERNAL_ERROR "Internal Server Error"

typedef struct request {
  char type[HTTP_METHOD_MAX_LEN_BYTES];
  char uri[HTTP_URI_MAX_LEN_BYTES];
} request;

void http_handler(int *client_socket);

#endif  // HTTP_SERVER_H_