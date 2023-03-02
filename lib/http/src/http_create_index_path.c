#include "http.h"

int http_create_index_path(char *path) { 
    if(!strcat(path, HTTP_DIR_INDEX)){
        return HTTP_ERROR;
    }

    return HTTP_SUCCESS;
}