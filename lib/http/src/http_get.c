
#include "http.h"

int http_get(int sock, request *req) {
  if (!req) {
    fprintf(stderr, "http[http_get()]: REQUEST is not provided\n");
    return HTTP_ERROR;
  }


  // struct timeval start;
  // gettimeofday(&start, NULL);

  // Create full path
  char full_path[HTTP_URI_MAX_LEN_BYTES + HTTP_ROOT_DIR_LEN];
  if (http_create_full_path(full_path, req->uri) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_get()]: Can't create full path to file\n");
    return HTTP_ERROR;
  }

  // Check is dir
  int is_dir = http_is_dir(full_path);

  // Resolve relative parts of path
  char *resolved_path[1024];
  char *res = realpath(full_path, resolved_path);
  DEB("\tResolved path %s\n", resolved_path);
  if (!res) {
    if (is_dir && errno != ENOTDIR) {
      return http_forbidden(sock);
    }

    return http_not_found(sock);
  }

  // Check root escaping
  if(http_check_root_escaping(resolved_path) != HTTP_SUCCESS) { 
    return http_forbidden(sock);
  }

  // check if index file needed
  if(is_dir){
    if(http_create_index_path(full_path) != HTTP_SUCCESS){
      return HTTP_ERROR;
    }
  }
  DEB("\tFile path result: %s\n", full_path);


  // Try open file
  FILE *file = fopen(full_path, "rb");

  if (!file) {
    switch (errno) {
      default: {
        perror("http[http_get()]: ");
        // return http_internal_error(sock);
        return HTTP_ERROR;
      }

      case ENOTDIR:
      case ENOENT: {
        if(is_dir){
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

  // get content type

  const char *content_type;
  if (http_content_type(&content_type, full_path) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_get()]: Can't get content-type of file\n");
    fclose(file);
    return HTTP_ERROR;
  }

  // char content_type[32];
  // if (http_content_type_magic(content_type, abs_path) != HTTP_SUCCESS) {
  //   fprintf(stderr, "http[http_get()]: Can't get content-type of file\n");
  //   fclose(file);
  //   return HTTP_ERROR;
  // }


  // Get file len
  off_t file_len = flength(file);
  DEB("\tfile_len(): %jd\n", file_len);

  if (file_len == HTTP_ERROR) {
    fprintf(stderr, "http[http_get()]: Can't get length of file\n");
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
  if (fsend_buff(sock, file) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_get()]: Can't send file\n");
    fclose(file);
    return HTTP_ERROR;
  }

  fclose(file);

  //   struct timeval stop;
  // gettimeofday(&stop, NULL);

  // DEB("time: %ld microsecs\n",
  // 1000000 * (stop.tv_sec - start.tv_sec) + stop.tv_usec - start.tv_usec);

  return HTTP_SUCCESS;
}