#include "http.h"

int fsend_buff(int sock, FILE *file) {
  int fd = -1;
  if ((fd = fileno(file)) < 0) {
    fprintf(stderr, "http[fsend_buff()]: Getting fd error\n");
    return HTTP_ERROR;
  }

  long file_len = flength(file);
  if (file_len == HTTP_ERROR) {
    fprintf(stderr, "http[fsend_buff()]: Can't get length of file\n");
    return HTTP_ERROR;
  }

  off_t off = 0;

  while (off < file_len) {
    size_t buff_len =
        file_len - off < FSEND_BUFF_BYTES ? file_len - off : FSEND_BUFF_BYTES;
    void *buff = mmap(NULL, buff_len, PROT_READ, MAP_SHARED | MAP_NORESERVE, fd,
                      off); 
    if (!buff) {
      perror("http[fsend_buff()]: ");
      return HTTP_ERROR;
    }

    ssize_t bytes_sended = send(sock, buff, buff_len, 0);
    if (bytes_sended < buff_len) {
      fprintf(stderr, "http[fsend_mmap()]: Sending file missed %ld bytes\n",
              file_len - off - bytes_sended);

      munmap(buff, buff_len);
      return HTTP_ERROR;
    }

    munmap(buff, buff_len);
    off += buff_len;
  }

  return HTTP_SUCCESS;
}