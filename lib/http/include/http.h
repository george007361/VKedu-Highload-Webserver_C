#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "http_internal.h"

#define HTTP_ERROR -1
#define HTTP_SUCCESS 0

int http_setup(const char *serv_name, const char *doc_root,
               const char *dir_idx);
void *http_handler(void *client_socket);

#endif  // HTTP_SERVER_H_
