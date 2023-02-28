#include "server.h"

int server_run(server *serv) {
  if (listen(serv->sock, serv->max_conn) < 0) {
    perror("server_run(): listen error:");
    return SERVER_ERROR;
  }

  return server_worker(serv);
}