#include "http.h"

void http_handler(int *client_socket) {
  int sock = *client_socket;
  DEB("http[http_handler()]: Socket %d\n", sock);

  // Read request from socket
  char req_raw[HTTP_REQUEST_MAX_LEN_BYTES];

  ssize_t req_raw_len =
      http_read_request(req_raw, HTTP_REQUEST_MAX_LEN_BYTES, sock);
  if (req_raw_len < 1) {
    fprintf(stderr, "http[http_handler()]: error while receiving request\n");
    http_internal_error(sock);

    http_close_safe(sock, 10);

    return;
  }

  // Parse request string
  request req;
  if (http_parse_request(&req, req_raw, req_raw_len) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_handler()]: error while parsing request\n");
    http_internal_error(sock);
    http_close_safe(sock, 10);

    return;
  }

  // decode uri
  http_decode_uri(req.uri);

  if (REQ_TYPE_IS(HTTP_GET)) {
    // Get request
    if (http_get(sock, &req) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);

      return;
    }
  } else if (REQ_TYPE_IS(HTTP_HEAD)) {
    // Head request
    if (http_head(sock, &req) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);

      return;
    }
  } else {
    // Unknown type
    if (http_unknown_method(sock) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);

      return;
    }
  }

  http_close_safe(sock, 10);
}