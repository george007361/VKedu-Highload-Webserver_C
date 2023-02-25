#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

void h(int s) {
  printf("Handler!!!\n");
  close(s);
}

int main(void) {
  printf("Hello web server!\n");

  server *serv = server_init(8003, 1000, h);

  server_run(serv);

  return EXIT_SUCCESS;
}