#include "server.h"

void server_destroy(server *serv) {
  if (!serv) {
    return;
  }
  server_stop(serv);
  thread_pool_destroy(serv->pool, POOL_STOP_NOW);
  close(serv->sock);
  free(serv);
}