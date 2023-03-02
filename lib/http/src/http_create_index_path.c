#include "http.h"

int http_create_index_path(char *path) {
  if (!strncat(path, HTTP_DIR_INDEX, HTTP_FILE_PATH_MAX_LEN - strlen(path))) {
    L_ERR_THR("http", "create_index_path", strerror(errno));
    return HTTP_ERROR;
  }

  return HTTP_SUCCESS;
}