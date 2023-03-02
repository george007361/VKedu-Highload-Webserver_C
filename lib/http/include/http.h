#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <stdlib.h>

#define SERVER_NAME "hl-web-server"
// #define HTTP_DOCUMENT_ROOT "/home/gg/Desktop/TP-3-sem/HL-Web-server-GIT"
#define HTTP_DOCUMENT_ROOT \
  "/home/gg/Desktop/TP-3-sem/HL-Web-server-GIT/http-test-suite-master"
#define HTTP_DIR_INDEX "index.html"
// #define HTTP_TIME_IT

#define HTTP_ERROR -1
#define HTTP_SUCCESS 0


void *http_handler(int *client_socket);

#include "http_internal.h"

#endif  // HTTP_SERVER_H_