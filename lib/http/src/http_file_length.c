
#include "http.h"

off_t http_file_length(FILE *file) {
  int fd = fileno(file);
  if (fd < 0) {
    L_DEB("http", "file_length", strerror(errno));
    return -1;
  }

  struct stat file_stat;
  if (fstat(fileno(file), &file_stat) < 0) {
    L_DEB("http", "file_length", strerror(errno));
    return -1;
  }

  return file_stat.st_size;
}
