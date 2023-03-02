#include "http.h"

int http_parse_request(request *req, char *raw) {
  if (!req) {
    L_ERR_THR("http", "parse_request", "Request dest not provided");
    return HTTP_ERROR;
  }

  if (!raw) {
    L_ERR_THR("http", "parse_request", "Request raw not provided");
    return HTTP_ERROR;
  }

  // GET /uri ...etc...

  // Достать тип запроса
  char *type_start = raw;
  char *type_end = strchr(type_start, ' ');
  if (!type_end) {
    L_ERR_THR("http", "parse_request", "Request TYPE not found");
    return HTTP_ERROR;
  }

  size_t type_len = type_end - type_start;
  if (!memcpy(req->type, raw, type_len)) {
    L_ERR_THR("http", "parse_request", "Request TYPE parsing error");
    return HTTP_ERROR;
  }
  req->type[type_len] = '\0';

  // Достать URI и query
  char *uri_start = type_end + 1;
  char *uri_end = strchr(uri_start, ' ');
  if (!uri_end) {
    L_ERR_THR("http", "parse_request", "Request URI not found");
    return HTTP_ERROR;
  }

  char *query_start = strchr(uri_start, '?');
  if (query_start && query_start < uri_end) {
    size_t query_len = uri_end - query_start;
    if (!memcpy(req->query, query_start, query_len)) {
      L_ERR_THR("http", "parse_request", "Request QUERY parsing error");
      return HTTP_ERROR;
    }
    req->query[query_len] = '\0';

    uri_end = query_start;
  } else {
    req->query[0] = '\0';
  }

  size_t uri_len = uri_end - uri_start;
  if (!memcpy(req->uri, uri_start, uri_len)) {
    L_ERR_THR("http", "parse_request", "Request URI parsing error");
    return HTTP_ERROR;
  }
  req->uri[uri_len] = '\0';

  return HTTP_SUCCESS;
}
