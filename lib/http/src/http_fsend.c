#include "http.h"

int http_fsend(int sock, FILE *file) {
  int fd = -1;
  if ((fd = fileno(file)) < 0) {
    L_ERR_THR("http", "fsend", "Cant get fd for file");
    return HTTP_ERROR;
  }

  ssize_t file_len = http_file_length(file);
  if (file_len == HTTP_ERROR) {
    L_ERR_THR("http", "fsend", "Cant get file length");
    return HTTP_ERROR;
  }

  off_t offset = 0;
  ssize_t bytes = 0;
  while (offset < file_len) {
    bytes = sendfile(sock, fd, &offset, file_len - offset);

    if (bytes < 0) {
      if (errno != EAGAIN) {
        L_INFO_THR("http", "fsend()",
                   "Error while sending file. Lost %ld bytes",
                   file_len - offset);
        break;
      }
      usleep(SEND_FILE_EAGAIN_TIMEOUT_US);
    }
  }

  return HTTP_SUCCESS;
}