#include "http.h"

#define REQ_TYPE_IS(val) !strcmp(req.type, val)

void http_handler(int *client_socket) {
  int sock = *client_socket;
  DEB("http[http_handler()]: Socket %d\n", sock);

  // Read request from socket
  char req_raw[HTTP_REQUEST_MAX_LEN_BYTES];

  ssize_t req_raw_len = http_read_request(req_raw, HTTP_REQUEST_MAX_LEN_BYTES, sock);
  if (req_raw_len < 1) {
    fprintf(stderr, "http[http_handler()]: error while receiving request\n");
    return;
  }

  // Parse request string
  request req;
  if (http_parse_request(&req, req_raw, req_raw_len) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_handler()]: error while parsing request\n");
    return;
  }

  if (REQ_TYPE_IS(HTTP_GET)) {
    // Get request
    http_get(&req);
  } else if (REQ_TYPE_IS(HTTP_HEAD)) {
    // Head request
    http_head(&req);
  } else {
    // Unknown type
  }
}