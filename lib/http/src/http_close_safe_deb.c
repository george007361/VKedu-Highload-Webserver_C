#include "http.h"
#include <assert.h>
int haveInput(int fd, double timeout) {
  int status;
  fd_set fds;
  struct timeval tv;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  tv.tv_sec = (long)timeout;                             // cast needed for C++
  tv.tv_usec = (long)((timeout - tv.tv_sec) * 1000000);  // 'suseconds_t'

  while (1) {
    if (!(status = select(fd + 1, &fds, 0, 0, &tv)))
      return 0;
    else if (status > 0 && FD_ISSET(fd, &fds))
      return 1;
    else if (status > 0)
      L_ERR_THR(NULL, NULL, "I am confused");
    else if (errno != EINTR)
      L_ERR_THR(NULL, NULL, "I am confused");
    // FatalError("select");  // tbd EBADF: man page "an error has occurred"
  }
}

int flushSocketBeforeClose(int fd, double timeout) {
  const double start = time(NULL) / 1000;
  char discard[99];
  assert(SHUT_WR == 1);
  if (shutdown(fd, 1) != -1)
    while (time(NULL) / 1000. < start + timeout)
      while (haveInput(fd, 0.01))  // can block for 0.01 secs
        if (!read(fd, discard, sizeof discard)) return 1;  // success!
  return 0;
}

void http_close_safe(int sock, int timeout) {
  L_DEB_THR("http", "close_safe", "Closing client socket with timeout %i sec",
            timeout);
  if (!flushSocketBeforeClose(sock, 2.0))  // can block for 2s
    printf("Warning: Cannot gracefully close socket\n");
  close(sock);
}