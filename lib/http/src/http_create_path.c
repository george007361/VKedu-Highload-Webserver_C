#include "http.h"

const int HTTP_ROOT_DIR_LEN = sizeof(HTTP_ROOT_DIR);

int http_create_path(char *path, char *uri) {
  path[0] = '\0';
  if (!strcat(path, HTTP_ROOT_DIR) || !strcat(path, uri)) {
    perror("http[http_fopen()]: ");
    return HTTP_ERROR;
  };

  // Check uri is dir
  if (path[strlen(path) - 1] == '/') {
    // add default file name
    if (!strcat(path, HTTP_DIR_INDEX)) {
      perror("http[http_fopen()]: ");
      return HTTP_ERROR;
    };
  }

  DEB("\t\tFile: %s\n", path);

  return HTTP_SUCCESS;
}