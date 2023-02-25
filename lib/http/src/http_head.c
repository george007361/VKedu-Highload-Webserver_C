#include "http.h"

int http_head(request *req) {
  if (!req) {
    fprintf(stderr, "http[http_get()]: req ptr is NULL\n");
    return HTTP_ERROR;
  }
  return HTTP_SUCCESS;
}
