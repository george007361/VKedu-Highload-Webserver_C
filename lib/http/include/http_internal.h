#ifndef HTTP_INTERNAL_H_
#define HTTP_INTERNAL_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

#define REQ_TYPE_IS(val) !strcmp(req.type, val)

// Supported types
#define HTTP_GET "GET"
#define HTTP_HEAD "HEAD"

// Response headers
#define HEADER_STATUS "HTTP/1.1 %d %s\r\n"
#define HEADER_SERVER "Server: %s\r\n"
#define HEADER_DATE "Date: %ld\r\n"
#define HEADER_CONNECTION "Connection: %s\r\n"
#define HEADER_CONTENT_TYPE "Content-Type: %s\r\n"
#define HEADER_CONTENT_LENGTH "Content-Length: %ld\r\n"
#define CRLF "\r\n"

#define CONNECTION_CLOSE "close"

// Response statuses and messages
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

#define HTTP_METHOD_MAX_LEN_BYTES 8
#define HTTP_URI_MAX_LEN_BYTES 128
typedef struct request {
  char type[HTTP_METHOD_MAX_LEN_BYTES];
  char uri[HTTP_URI_MAX_LEN_BYTES];
} request;

extern const int HTTP_ROOT_DIR_LEN;

// Funcs for working with file
int http_create_path(char *path, char *uri);
off_t flength(FILE *file);
int fsend_buff(int sock, FILE *file);
int fsend_mmap(int sock, FILE *file);
int http_content_type(const char **content_type, char *uri);

// Processors for HTTP resposes
#define HTTP_RESPONCE_HEADERS_LEN_BYTES 256
#define FSEND_BUFF_BYTES 4096

int http_get(int sock, request *req);
int http_head(int sock, request *req);
int http_forbidden(int sock);
int http_internal_error(int sock);
int http_not_found(int sock);
int http_unknown_method(int sock);

// Request processors
#define HTTP_REQUEST_MAX_LEN_BYTES 128

ssize_t http_read_request(char *buff, const ssize_t buff_len, int sock);
int http_parse_request(request *req, char *raw);
void http_close_safe(int sock, int timeout);
int http_decode_uri(char *uri);

#endif  // HTTP_INTERNAL_H_