#include "http.h"

int http_head(int sock, request *req) {
  if (!req) {
    L_ERR_THR("http", "head", "Request not provided");
    return HTTP_ERROR;
  }

  // Create full path
  char full_path[HTTP_FILE_PATH_MAX_LEN];
  if (http_create_full_path(full_path, req->uri) != HTTP_SUCCESS) {
    L_ERR_THR("http", "head", "Can't create full path to file");
    return HTTP_ERROR;
  }

  // Check is dir
  int is_dir = http_is_dir(full_path);

  // Resolve relative parts of path
  char resolved_path[HTTP_FILE_PATH_MAX_LEN];
  if (!realpath(full_path, resolved_path)) {
    if (is_dir && errno != ENOTDIR) {
      return http_forbidden(sock);
    }
    return http_not_found(sock);
  }

  // Check root escaping
  if (http_check_root_escaping(resolved_path) != HTTP_SUCCESS) {
    return http_forbidden(sock);
  }

  // check if index file needed
  if (is_dir) {
    if (http_create_index_path(full_path) != HTTP_SUCCESS) {
      L_ERR_THR("http", "head", "Can't create path to index file of directory");
      return HTTP_ERROR;
    }
  }
  L_DEB_THR("http", "head", "File path is %s", full_path);

  // Try open file
  FILE *file = fopen(full_path, "rb");

  if (!file) {
    switch (errno) {
      default: {
        L_ERR_THR("http", "head", strerror(errno));
        return HTTP_ERROR;
      }

      case ENOTDIR:
      case ENOENT: {
        if (is_dir) {
          return http_forbidden(sock);
        }

        return http_not_found(sock);
      }

      case EACCES: {
        return http_forbidden(sock);
      }
    }
  }

  // If opened successfully

  // head content type
  const char *content_type;
  if (http_content_type(&content_type, full_path) != HTTP_SUCCESS) {
    L_ERR_THR("http", "head", "Can't head content-type of file");
    fclose(file);
    return HTTP_ERROR;
  }

  // Get file len
  off_t file_len;
  if ((file_len = http_file_length(file)) < 0) {
    L_ERR_THR("http", "head", "Can't head file length");
    fclose(file);
    return HTTP_ERROR;
  }
  L_DEB_THR("http", "head", "File length is %ld", file_len);

  // Write response
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
                       HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION
                           HEADER_CONTENT_LENGTH HEADER_CONTENT_TYPE CRLF,
                       STATUS_OK, MESSAGE_OK, SERVER_NAME, time(NULL),
                       CONNECTION_CLOSE, file_len, content_type);

  if (!bytes) {
    L_DEB_THR("http", "head", "Can't create headers: %s", strerror(errno));
    fclose(file);
    return HTTP_ERROR;
  }

  // Send headers
  if (send(sock, responce, bytes, 0) < bytes) {
    L_DEB_THR("http", "head", "Can't send headers: %s", strerror(errno));
    fclose(file);
    return HTTP_ERROR;
  }

  fclose(file);
  return HTTP_SUCCESS;
}
