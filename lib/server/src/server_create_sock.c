#include "server.h"

int server_create_sock(const unsigned short port) {
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // | SOCK_NONBLOCK
  if (sock < 0) {
    L_ERR("server", "create_sock", strerror(errno));
    return SERVER_ERROR;
  }

  int optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    L_ERR("server", "create_sock", strerror(errno));
    close(sock);
    return SERVER_ERROR;
  }

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0) {
    L_ERR("server", "create_sock", strerror(errno));
    close(sock);
    return SERVER_ERROR;
  }

  struct sockaddr_in serv_info;
  serv_info.sin_family = AF_INET;
  serv_info.sin_port = htons(port);
  serv_info.sin_addr.s_addr = htonl(INADDR_ANY);  // = ip_to_num;  //

  if (bind(sock, (struct sockaddr *)&serv_info, sizeof(serv_info)) < 0) {
    L_ERR("server", "create_sock", strerror(errno));
    close(sock);
    return SERVER_ERROR;
  }

  L_DEB("server", "create_sock", "Server socket created successfully");

  return sock;
}
