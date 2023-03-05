
#include "http.h"

int fsend_mmap(int sock, FILE *file) {
  int fd = -1;
  if ((fd = fileno(file)) < 0) {
    fprintf(stderr, "http[fsend_mmap()]: Getting fd error\n");
    return HTTP_ERROR;
  }

  long file_len = http_file_length(file);
  if (file_len == HTTP_ERROR) {
    fprintf(stderr, "http[fsend_mmap()]: Can't get length of file\n");
    return HTTP_ERROR;
  }

  void *adr = mmap(NULL, file_len, PROT_READ, MAP_SHARED | MAP_NORESERVE, fd,
                   0);  // i get the
  if (!adr) {
    perror("http[fsend_mmap()]: ");
    return HTTP_ERROR;
  }

  ssize_t bytes_payload = send(sock, adr, file_len, 0);
  if (bytes_payload < file_len) {
    fprintf(stderr, "http[fsend_mmap()]: Sending file missed %ld bytes\n",
            file_len - bytes_payload);

    munmap(adr, file_len);
    return HTTP_ERROR;
  }

  munmap(adr, file_len);

  return HTTP_SUCCESS;
}
