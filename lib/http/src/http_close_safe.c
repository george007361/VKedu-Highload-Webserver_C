#include "http.h"

void http_close_safe(int sock, int timeout) {
  L_DEB_THR("http", "close_safe", "Closing client socket with timeout %i sec",
            timeout);

  shutdown(sock, SHUT_WR); /* Send EOF to web browser */

  static const int tmp_buff_size = 128;
  char tmp_buff[tmp_buff_size];
  time_t begin = time(NULL);

  while (recv(sock, tmp_buff, tmp_buff_size, 0) > 0 &&
         time(NULL) - begin < timeout)
    ;
  /* Read and discard until we see the browser's EOF (or
                       an error) */

  L_DEB_THR("http", "close_safe", "Connection closed");
  close(sock);
}