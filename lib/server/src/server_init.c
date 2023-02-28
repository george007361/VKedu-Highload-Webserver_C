#include "server.h"

server *server_init(const unsigned short port, const int max_conn,
                    const int max_threads, void *(*handler)(int *)) {
  if (!handler) {
    fprintf(stderr, "server_init(): handler not provided\n");
    return NULL;
  }

  server *serv = (server *)malloc(sizeof(server));
  if (!serv) {
    fprintf(stderr, "server_init(): Cant allocate mem for server\n");
    return NULL;
  }

  serv->handler = handler;
  serv->max_conn = max_conn;
  serv->sock = server_create_sock(port);

  serv->pool = thread_pool_init(max_threads);
  if (!serv->pool) {
    fprintf(stderr, "server_init(): Cant create thread pool\n");
    free(serv);
    return NULL;
  }

  if (serv->sock < 0) {
    fprintf(stderr, "server_init(): Cant open socket for server\n");
    free(serv);
    return NULL;
  }

  return serv;
}