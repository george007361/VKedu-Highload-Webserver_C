#include <sys/mman.h>

#include "http.h"

int http_get(int sock, request *req) {
  if (!req) {
    fprintf(stderr, "http[http_get()]: req ptr is NULL\n");
    return HTTP_ERROR;
  }

  // Create abs path
  char abs_path[HTTP_URI_MAX_LEN_BYTES + HTTP_ROOT_DIR_LEN];
  if (http_create_path(abs_path, req->uri) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_get()]: Can't create absolute path to file\n");
    return HTTP_ERROR;
  }

  // Try open file
  FILE *file = fopen(abs_path, "rb");

  if (!file) {
    switch (errno) {
      default: {
        perror("http[http_get()]: ");
        return http_internal_error(sock);
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

  // TODO: get content type

  // Get file len
  long file_len = flength(file);
  if (file_len == HTTP_ERROR) {
    fprintf(stderr, "http[http_get()]: Can't get length of file\n");
    fclose(file);
    return HTTP_ERROR;
  }

  // Write response
  char responce[HTTP_RESPONCE_HEADERS_LEN_BYTES];

  int bytes = snprintf(responce, HTTP_RESPONCE_HEADERS_LEN_BYTES,
                       HEADER_STATUS HEADER_SERVER HEADER_DATE HEADER_CONNECTION
                           HEADER_CONTENT_LENGTH CRLF,
                       STATUS_OK, MESSAGE_OK, SERVER_NAME, time(NULL),
                       CONNECTION_CLOSE, file_len);

  //   DEB("\tResponce:\n%s %d\n\t------\n", responce, bytes);

  if (!bytes) {
    perror("http[http_get()]: ");
    fclose(file);
    return HTTP_ERROR;
  }

  // Send headers
  if (send(sock, responce, bytes, 0) < bytes) {
    perror("http[http_get()]: ");
    fclose(file);
    return HTTP_ERROR;
  }

  // Send content
  int fd = -1;
  if ((fd = fileno(file)) < 0) {
    fprintf(stderr, "http[http_get()]: Getting fd error\n");
    fclose(file);
    return HTTP_ERROR;
  }

  void *adr = mmap(NULL, file_len, PROT_READ, MAP_SHARED, fd, 0);  // i get the
  if (!adr) {
    perror("http[http_get()]: ");
    fclose(file);
    return HTTP_ERROR;
  }

  ssize_t bytes_payload = send(sock, adr, file_len, 0);
  if (bytes_payload < file_len) {
    fprintf(stderr, "http[http_get()]: Sending file missed %ld bytes\n",
            file_len - bytes_payload);

    fclose(file);
    munmap(adr, file_len);
    return HTTP_ERROR;
  }

  //

  fclose(file);
  munmap(adr, file_len);

  return HTTP_SUCCESS;
}