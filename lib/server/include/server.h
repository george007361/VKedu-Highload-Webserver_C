#ifndef SERVER_H_
#define SERVER_H_

#include "server_internal.h"

#define SERVER_ERROR -1
#define SERVER_OK 0

#define SERVER_MAX_CONN_DEF 100

server *server_init(const unsigned short port, const int max_conn,
                    const int max_threads, void *(*handler)(void *client_socket));
void server_destroy(server *serv);
int server_run(server *serv);
void server_stop(server *serv);

#endif  // SERVER_H_