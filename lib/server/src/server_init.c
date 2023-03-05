#include "server.h"

server *server_init(const unsigned short port, const int max_conn,
                    const int max_threads,
                    void *(*handler)(void *client_socket)) {
  L_INFO("server", "init", "Creating server. Port: %u, Max worker threads: %i",
         port, max_threads);

  server *serv = (server *)malloc(sizeof(server));
  if (!serv) {
    L_ERR("server", "init", "Can't allocate memory for server");
    return NULL;
  }

  if (!handler) {
    L_ERR("server", "init", "Request handler not provided");
    free(serv);
    return NULL;
  }
  serv->handler = handler;

  if (max_conn < 1) {
    L_INFO("server", "init", "Max connections not provided, using default",
           SERVER_MAX_CONN_DEF);
    serv->max_conn = SERVER_MAX_CONN_DEF;
  } else {
    serv->max_conn = max_conn;
  }

  serv->sock = server_create_sock(port);
  if (serv->sock < 0) {
    L_ERR("server", "init", "Can't create server socket");
    free(serv);
    return NULL;
  }

  serv->pool = thread_pool_init(max_threads);
  if (!serv->pool) {
    L_ERR("server", "init", "Can't create thread pool for server");
    free(serv);
    return NULL;
  }

  L_DEB("server", "init", "Creating server success");
  return serv;
}
