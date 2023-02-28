#include "server.h"

void server_stop(server *serv) {
  shutdown(serv->sock, SHUT_RDWR);
  thread_pool_wait(serv->pool, POOL_STOP_WAIT);
}
