#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "server.h"

#define DEBUG

void handler_middleware(int client_socket) {
  int *p = malloc(sizeof(int));
  *p = client_socket;
  http_handler(p);
  free(p);
  close(client_socket);
}

int main(void) {
  printf("Hello web server!\n");

  server *serv = server_init(8003, 1000, handler_middleware);

  server_run(serv);

  server_destroy(serv);

  return EXIT_SUCCESS;
}