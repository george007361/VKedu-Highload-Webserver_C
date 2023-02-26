#include "server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int server_create_sock(const unsigned short port) {
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // | SOCK_NONBLOCK
  if (sock < 0) {
    perror("server_create_sock(): ");
    return SERVER_ERROR;
  }

  int optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    perror("server_create_sock(): ");
    close(sock);
    return SERVER_ERROR;
  }

  struct sockaddr_in serv_info;
  serv_info.sin_family = AF_INET;
  serv_info.sin_port = htons(port);
  serv_info.sin_addr.s_addr = htonl(INADDR_ANY);  // = ip_to_num;  //

  if (bind(sock, (struct sockaddr *)&serv_info, sizeof(serv_info)) < 0) {
    perror("server_create_sock(): ");
    close(sock);
    return SERVER_ERROR;
  }

  return sock;
}

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
        serv->handler(client_sock);
        break;
      } else {
        printf("Server[connection-manager]: No handler provided\n");
        close(client_sock);
      }
    }
  }

  return SERVER_OK;
}

int server_run(server *serv) {
  if (listen(serv->sock, serv->max_conn) < 0) {
    perror("server_run(): listen error:");
    return SERVER_ERROR;
  }

  return server_worker(serv);
}

void server_stop(server *serv) { shutdown(serv->sock, SHUT_RDWR); }

server *server_init(const unsigned short port, const int max_conn,
                    void (*handler)(int)) {
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

  if (serv->sock < 0) {
    fprintf(stderr, "server_init(): Cant open socket for server\n");
    free(serv);
    return NULL;
  }

  return serv;
}

void server_destroy(server *serv) {
  if (!serv) {
    return;
  }
  server_stop(serv);
  close(serv->sock);

  free(serv);
}
