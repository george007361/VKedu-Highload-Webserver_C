#include "server.h"

int server_worker(server *serv) {
  while (1) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int client_sock = accept(serv->sock, (struct sockaddr *)&client_addr,
                             (socklen_t *)&client_addr_len);
    if (client_sock > 0) {
      printf("Server[connection-manager]: New connection to socket %d\n",
             client_sock);
      if (serv->handler) {
        int *ptr_client_sock = (int *)malloc(sizeof(int));
        *ptr_client_sock = client_sock;
        thread_pool_add_task(serv->pool, serv->handler,
                             (void *)ptr_client_sock);
      } else {
        printf("Server[connection-manager]: No handler provided\n");
        close(client_sock);
      }
    }
  }

  return SERVER_OK;
}
