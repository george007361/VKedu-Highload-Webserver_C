#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "server.h"

int main(void) {
  printf("Hello async web server!\n");

  server *serv = server_init(8003, 1000, 1, http_handler);

  server_run(serv);

  server_destroy(serv);

  return EXIT_SUCCESS;
}