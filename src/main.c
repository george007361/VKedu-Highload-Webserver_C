#include "http.h"
#include "server.h"

int main(void) {
  server *serv = server_init(8003, 1000, 1, http_handler);
  server_run(serv);
  server_destroy(serv);
  return EXIT_SUCCESS;
}