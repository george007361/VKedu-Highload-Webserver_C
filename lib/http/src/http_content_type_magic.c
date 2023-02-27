#include <magic.h>

#include "http.h"

int http_content_type_magic(char *content_type, char *path) {
  if (!content_type || !path) {
    fprintf(stderr, "http[http_content_type_magic()]: Invalid params\n");
    return HTTP_ERROR;
  }

  magic_t magic = magic_open(MAGIC_MIME_TYPE);
  if (!magic) {
    fprintf(stderr, "http[http_content_type_magic()]: Can't open magic\n");
    return HTTP_ERROR;
  }
  
  if(magic_load(magic, NULL)) {
    fprintf(stderr, "http[http_content_type_magic()]: Can't load magic\n");
    return HTTP_ERROR;
  }

  
  const char *res = magic_file(magic, path);
  strcpy(content_type, res);

  DEB("http[http_content_type_magic()]: %s\n", content_type);
  magic_close(magic);

  return HTTP_SUCCESS;
}