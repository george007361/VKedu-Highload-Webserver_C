#include "http.h"

int http_head(int sock, request *req) {
  if (!req) {
    fprintf(stderr, "http[http_head()]: req ptr is NULL\n");
    return HTTP_ERROR;
  }

  // Create abs path
  char abs_path[HTTP_URI_MAX_LEN_BYTES + HTTP_ROOT_DIR_LEN];
  if (http_create_path(abs_path, req->uri) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_head()]: Can't create absolute path to file\n");
    return HTTP_ERROR;
  }

  // Try open file
  FILE *file = fopen(abs_path, "rb");

  if (!file) {
    switch (errno) {
      default: {
        perror("http[http_head()]: ");
        return HTTP_ERROR;
      }

      case ENOTDIR:
      case ENOENT: {
        return http_not_found(sock);
      }

      case EACCES: {
        return http_forbidden(sock);
      }
    }
  }

  // If opened successfully

  // get content type
  char *content_type;
  if (http_content_type(&content_type, abs_path) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_head()]: Can't get content-type of file\n");
    fclose(file);
    return HTTP_ERROR;
  }

  // Get file len
  long file_len = flength(file);
  if (file_len == HTTP_ERROR) {
    fprintf(stderr, "http[http_head()]: Can't get length of file\n");
    fclose(file);
    return HTTP_ERROR;
  }

  // Write response
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
                       HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION
                           HEADER_CONTENT_LENGTH HEADER_CONTENT_TYPE CRLF,
                       STATUS_OK, MESSAGE_OK, SERVER_NAME, time(NULL),
                       CONNECTION_CLOSE, file_len, content_type);

  //   DEB("\tResponce:\n%s %d\n\t------\n", responce, bytes);

  if (!bytes) {
    perror("http[http_head()]: ");
    fclose(file);
    return HTTP_ERROR;
  }

  if (send(sock, responce, bytes, 0) < bytes) {
    perror("http[http_head()]: ");
    fclose(file);
    return HTTP_ERROR;
  }

  fclose(file);

  return HTTP_SUCCESS;
}
