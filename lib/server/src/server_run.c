#include "server.h"

int server_run(server *serv) {
  L_INFO("server", "run", "Starting server");
  if (listen(serv->sock, serv->max_conn) < 0) {
    L_ERR("server", "run", strerror(errno));
    return SERVER_ERROR;
  }

  L_DEB("server", "run", "Starting server success");
  return server_worker(serv);
}