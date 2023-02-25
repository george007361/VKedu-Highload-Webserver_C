#ifndef SERVER_H_
#define SERVER_H_

#define SERVER_ERROR -1
#define SERVER_OK 0

#ifdef DEBUG
#define DEB printf
#else
#define DEB(...)
#endif

typedef struct server {
  int sock;
  int max_conn;
  void (*handler)(int);
} server;

server *server_init(const unsigned short port, const int max_conn,
                    void (*handler)(int));
void server_destroy(server *serv);
int server_run(server *serv);
void server_stop(server *serv);

#endif  // SERVER_H_