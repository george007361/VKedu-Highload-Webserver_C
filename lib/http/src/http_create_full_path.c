#include "http.h"

int http_create_full_path(char *dest, char *uri) {
  dest[0] = '\0';
  if (!strncat(dest, HTTP_DOCUMENT_ROOT, HTTP_FILE_PATH_MAX_LEN) ||
      !strncat(dest, uri, HTTP_FILE_PATH_MAX_LEN - strlen(dest))) {
    L_ERR_THR("http", "create_full_path", strerror(errno));
    return HTTP_ERROR;
  };

  L_DEB_THR("http", "create_full_path", "Full path is %s", dest);
  return HTTP_SUCCESS;
}