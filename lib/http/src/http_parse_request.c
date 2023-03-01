#include "http.h"

int http_parse_request(request *req, char *raw) {
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
  DEB("\tRequest type: %s\n", req->type);

  // Достать URI и query
  char *uri_start = type_end + 1;
  char *uri_end = strchr(uri_start, ' ');
  if (!uri_end) {
    fprintf(stderr, "http[parse_request()]: request URI not found\n");
    return HTTP_ERROR;
  }

  char *query_start = strchr(uri_start, '?');
  if (query_start && query_start < uri_end) {
    size_t query_len = uri_end - query_start;
    if (!memcpy(req->query, query_start, query_len)) {
      fprintf(stderr, "http[parse_request()]: request query parsing error\n");
      return HTTP_ERROR;
    }
    req->query[query_len] = '\0';

    uri_end = query_start;
  } else {
    req->query[0] = '\0';
  }
  DEB("\tRequest query: %s\n", req->query);

  size_t uri_len = uri_end - uri_start;
  if (!memcpy(req->uri, uri_start, uri_len)) {
    fprintf(stderr, "http[parse_request()]: request URI parsing error\n");
    return HTTP_ERROR;
  }
  req->uri[uri_len] = '\0';
  DEB("\tRequest URI: %s\n", req->uri);

  return HTTP_SUCCESS;
}
