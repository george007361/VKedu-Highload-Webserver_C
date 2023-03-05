#include "server.h"

void server_stop(server *serv) {
  L_INFO("server", "stop", "Stopping server");

  shutdown(serv->sock, SHUT_RDWR);

  L_INFO("server", "stop", "Waiting while opened connections will be closed");
  thread_pool_wait(serv->pool, POOL_STOP_WAIT);

  L_DEB("server", "stop", "Stopping server success");
}
