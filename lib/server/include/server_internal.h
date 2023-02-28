#ifndef SERVER_INTERNAL_H_
#define SERVER_INTERNAL_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "thread_pool.h"

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

typedef struct server {
  int sock;
  int max_conn;
  thread_pool *pool;
  void *(*handler)(int *);
} server;

int server_create_sock(const unsigned short port);
int server_worker(server *serv);

#endif  // SERVER_INTERNAL_H_