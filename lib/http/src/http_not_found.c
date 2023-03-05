#include "http.h"

int http_not_found(int sock) {
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(
      responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
      HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION CRLF CRLF,
      STATUS_NOT_FOUND, MESSAGE_NOT_FOUND, HTTP_SERVER_NAME, time(NULL),
      CONNECTION_CLOSE);

  if (!bytes) {
    L_ERR_THR("http", "not_found", strerror(errno));
    return HTTP_ERROR;
  }

  if (send(sock, responce, bytes, 0) < bytes) {
    L_ERR_THR("http", "not_found", strerror(errno));
    return HTTP_ERROR;
  }

  return HTTP_SUCCESS;
}
