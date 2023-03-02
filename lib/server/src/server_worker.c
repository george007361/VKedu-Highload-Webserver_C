#include "server.h"

int server_worker(server *serv) {
  while (1) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_sock = accept(serv->sock, (struct sockaddr *)&client_addr,
                             (socklen_t *)&client_addr_len);
    if (client_sock > 0) {
      L_INFO("server", "worker", "New connection handled");
      if (serv->handler) {
        int *ptr_client_sock = (int *)malloc(sizeof(int));
        *ptr_client_sock = client_sock;
        thread_pool_add_task(serv->pool, serv->handler,
                             (void *)ptr_client_sock);
      } else {
        L_INFO("server", "worker", "No handler provided to server. Closing connection");
        close(client_sock);
      }
    }
  }

  return SERVER_OK;
}
