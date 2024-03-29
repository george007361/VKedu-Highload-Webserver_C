#ifndef SERVER_INTERNAL_H_
#define SERVER_INTERNAL_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "logger.h"
#include "thread_pool.h"

typedef struct server {
  int sock;
  int max_conn;
  thread_pool *pool;
  void *(*handler)(void *client_sock);
} server;

int server_create_sock(const unsigned short port);
int server_worker(server *serv);

#endif  // SERVER_INTERNAL_H_
