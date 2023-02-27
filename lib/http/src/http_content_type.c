#include "http.h"
struct content_type {
  char *content_type;
  char *extensions[10];
};

static const char *unknown_content_type = "application/octet-stream";

static const struct content_type content_types[] = {
    {"text/html", {"html", "htm", NULL}},
    {"text/css", {"css", NULL}},
    {"text/javascript", {"js", NULL}},
    {"image/png", {"png", NULL}},
    {"image/jpeg", {"jpeg", "jpg", NULL}},
    {"image/gif", {"gif", NULL}},
    {"application/x-shockwave-flash", {"swf", NULL}}};

int http_content_type(char **content_type, char *uri) {
  if (!uri && !content_type) {
    fprintf(stderr, "http[http_content_type()]: Invalid arguments\n");
    return HTTP_ERROR;
  }

  char *ptr_dot;
  if (!(ptr_dot = strchr(uri, '.'))) {
    *content_type = unknown_content_type;
    return HTTP_SUCCESS;
  }

  char *ptr_ext = ptr_dot + 1;
  int found = 0;
  for (int i = 0; !found && content_types[i].content_type; ++i) {
    // DEB("Trying %s\n", content_types[i].content_type);
    for (int j = 0; !found && content_types[i].extensions[j]; ++j) {
      // DEB("\t%s ", content_types[i].extensions[j]);
      if (strstr(ptr_ext, content_types[i].extensions[j])) {
        // DEB("\tv\n");
        found = 1;
        *content_type = content_types[i].content_type;
      }
      // else {
      // DEB("\tx\n");
      // }
    }
  }

  if (!found) {
    *content_type = unknown_content_type;
  }

  return HTTP_SUCCESS;
}