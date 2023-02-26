#include "http.h"

int http_forbidden(int sock) {
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(
      responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
      HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION CRLF CRLF,
      STATUS_FORBIDDEN, MESSAGE_FORBIDDEN, SERVER_NAME, time(NULL),
      CONNECTION_CLOSE);

  //   DEB("\tResponce:\n%s %d\n\t------\n", responce, bytes);

  if (!bytes) {
    perror("http[http_forbidden()]: ");
    return HTTP_ERROR;
  }

  if (send(sock, responce, bytes, 0) < bytes) {
    perror("http[http_forbidden()]: ");
    return HTTP_ERROR;
  }

  return HTTP_SUCCESS;
}