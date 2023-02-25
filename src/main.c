#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "http.h"
#include "server.h"

void h(int s) {
  printf("Handler!!!\n");
  int *p = malloc(sizeof(int));
  *p = s;
  http_handler(p);
  close(s);
}

int main(void) {
  printf("Hello web server!\n");

  server *serv = server_init(8003, 1000, h);

  server_run(serv);

  return EXIT_SUCCESS;
}