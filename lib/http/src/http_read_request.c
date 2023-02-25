#include "http.h"

ssize_t http_read_request(char *buff, const ssize_t buff_len, int sock) {
  ssize_t bytes = recv(sock, buff, buff_len - 1, 0);
  if (bytes < 0) {
    perror("http[read_requst]: ");
    return HTTP_ERROR;
  }

  buff[bytes] = '\0';

  return bytes;
}