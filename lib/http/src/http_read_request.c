#include "http.h"

ssize_t http_read_request(char *buff, const ssize_t buff_len, int sock) {
  fd_set read_set;
  struct timeval timeout = {0};
  timeout.tv_sec = HTTP_WAIT_REQUEST_S;
  timeout.tv_usec = HTTP_WAIT_REQUEST_US;
  FD_ZERO(&read_set);
  FD_SET(sock, &read_set);

  int r = select(sock + 1, &read_set, NULL, NULL, &timeout);

  if (r <= 0) {
    L_ERR_THR("http", "read_request", "Socket reding state unavailable: errno=%i, msg: %s", errno, strerror(errno));
    return -1;
  }

  ssize_t bytes = recv(sock, buff, buff_len - 1, 0);
  if (bytes < 0) {
    L_ERR_THR("http", "read_request", "Error while reading from socket");
    return -1;
  }

  if (bytes == 0) {
    L_ERR_THR("http", "read_request", "Client closed connetion");
    return -1;
  }

  buff[bytes] = '\0';

  return bytes;
}