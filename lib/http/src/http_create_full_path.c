#include "http.h"

const int HTTP_ROOT_DIR_LEN = sizeof(HTTP_ROOT_DIR);

int http_create_full_path(char *dest, char *uri) {
  dest[0] = '\0';
  if (!strcat(dest, HTTP_ROOT_DIR) || !strcat(dest, uri)) {
    perror("http[http_fopen()]: ");
    return HTTP_ERROR;
  };

  
  DEB("\t\tFULL PATH: %s\n", dest);

  return HTTP_SUCCESS;
}