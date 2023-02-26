
#include "http.h"

off_t flength(FILE *file) {
  int fd = fileno(file);
  if (fd < 0) {
    perror("flength(): ");
    return HTTP_ERROR;
  }

  struct stat file_stat;
  if (fstat(fileno(file), &file_stat) < 0) {
    perror("flenght(): ");
    return HTTP_ERROR;
  }
  return file_stat.st_size;
}
