#include "http.h"

int http_check_root_escaping(char *resolved_path) { 
    return strstr(resolved_path, HTTP_ROOT_DIR) ?  HTTP_SUCCESS : HTTP_ERROR;
}