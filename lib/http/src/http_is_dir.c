#include "http.h"

int http_is_dir(char *path) { return path[strlen(path) - 1] == '/'; }
