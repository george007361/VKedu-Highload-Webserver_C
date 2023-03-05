#ifndef HTTP_INTERNAL_H_
#define HTTP_INTERNAL_H_

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "logger.h"

// Config
#define HTTP_WAIT_REQUEST_S 5
#define HTTP_WAIT_REQUEST_US 0

#define HTTP_REQUEST_MAX_LEN_BYTES 256
#define HTTP_RESPONCE_HEADERS_LEN_BYTES 256

#define HTTP_METHOD_MAX_LEN_BYTES 8
#define HTTP_URI_MAX_LEN_BYTES 128
#define HTTP_QUERY_MAX_LEN_BYTES 64

#define FSEND_BUFF_BYTES 4096
#define HTTP_PATH_MAX 256

#define HTTP_SERV_NAME_MAX 32
#define HTTP_INDEX_MAX 16

#define SEND_FILE_EAGAIN_TIMEOUT_US 200

typedef struct http_config {
  char server_name[HTTP_SERV_NAME_MAX];
  char document_root[HTTP_PATH_MAX];
  char dir_index[HTTP_INDEX_MAX];
} http_config_t;

http_config_t http_config(http_config_t *set);
#define HTTP_SERVER_NAME http_config(NULL).server_name
#define HTTP_DOCUMENT_ROOT http_config(NULL).document_root
#define HTTP_DIR_INDEX http_config(NULL).dir_index

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

typedef struct request {
  char type[HTTP_METHOD_MAX_LEN_BYTES];
  char uri[HTTP_URI_MAX_LEN_BYTES];
  char query[HTTP_QUERY_MAX_LEN_BYTES];
} request;

static const int HTTP_DOCUMENT_ROOT_LEN = sizeof(HTTP_DOCUMENT_ROOT);

// Funcs for working with file
int http_create_full_path(char *dest, char *uri);
off_t http_file_length(FILE *file);
// int fsend_buff(int sock, FILE *file);
// int fsend_mmap(int sock, FILE *file);
int http_fsend(int sock, FILE *file);
int http_content_type(const char **content_type, char *uri);
int http_is_dir(char *path);
int http_check_root_escaping(char *resolved_path);
int http_create_index_path(char *path);

// Processors for HTTP resposes
int http_get(int sock, request *req);
int http_head(int sock, request *req);
int http_forbidden(int sock);
int http_internal_error(int sock);
int http_not_found(int sock);
int http_unknown_method(int sock);

// Request processors
ssize_t http_read_request(char *buff, const ssize_t buff_len, int sock);
int http_parse_request(request *req, char *raw);
void http_close_safe(int sock, int timeout);
int http_decode_request(request *req);

#define REQ_TYPE_IS(val) !strcmp(req.type, val)

#ifdef HTTP_TIME_IT
#include "sys/time.h"
#include "time.h"

#define SET_TIMER       \
  struct timeval start; \
  gettimeofday(&start, NULL);

#define TIME_IT(func, timer_name)                                      \
  {                                                                    \
    struct timeval stop;                                               \
    gettimeofday(&stop, NULL);                                         \
    L_INFO_THR("http", func, "Timer %s: %ld us", timer_name,           \
               1000000 * (stop.tv_sec - start.tv_sec) + stop.tv_usec - \
                   start.tv_usec);                                     \
  }

#else
#define SET_TIMER void;
#define TIME_IT(...)
#endif

#endif  // HTTP_INTERNAL_H_