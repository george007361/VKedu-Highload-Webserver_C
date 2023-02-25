#include "http.h"

int http_parse_request(request *req, char *raw, const ssize_t len) {
  if (!req) {
    fprintf(stderr, "http[parse_request()]: request ptr is NULL\n");
    return HTTP_ERROR;
  }

  if (!raw) {
    fprintf(stderr, "http[parse_request()]: raw ptr is NULL\n");
    return HTTP_ERROR;
  }

  // GET /uri ...etc...

  // Достать тип запроса
  char *type_start = raw;
  char *type_end = strchr(type_start, ' ');
  if (!type_end) {
    fprintf(stderr, "http[parse_request()]: request TYPE not found\n");
    return HTTP_ERROR;
  }

  size_t type_len = type_end - type_start;
  if (!memcpy(req->type, raw, type_len)) {
    fprintf(stderr, "http[parse_request()]: request TYPE parsing error\n");
    return HTTP_ERROR;
  }
  req->type[type_len] = '\0';

  // Достать URI
  char *uri_start = type_end + 1;
  char *uri_end = strchr(uri_start, ' ');
  if (!uri_end) {
    fprintf(stderr, "http[parse_request()]: request URI not found\n");
    return HTTP_ERROR;
  }
  size_t uri_len = uri_end - uri_start;
  if (!memcpy(req->uri, uri_start, uri_len)) {
    fprintf(stderr, "http[parse_request()]: request URI parsing error\n");
    return HTTP_ERROR;
  }
  req->uri[uri_len] = '\0';

  return HTTP_SUCCESS;
}
