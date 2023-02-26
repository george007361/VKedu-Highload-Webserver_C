#include <time.h>

#include "http.h"

int http_internal_error(int sock) {
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(
      responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
      HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION CRLF CRLF,
      STATUS_INTERNAL_ERROR, MESSAGE_INTERNAL_ERROR, SERVER_NAME, time(NULL),
      CONNECTION_CLOSE);

  //   DEB("\tResponce:\n%s %d\n\t------\n", responce, bytes);

  if (!bytes) {
    perror("http[http_internal_error()]: ");
    return HTTP_ERROR;
  }

  if (send(sock, responce, bytes, 0) < bytes) {
    perror("http[http_internal_error()]: ");
    return HTTP_ERROR;
  }

  return HTTP_SUCCESS;
}