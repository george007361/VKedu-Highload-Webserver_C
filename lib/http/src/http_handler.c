#include "http.h"

void *http_handler(void *client_socket) {
  int sock = *(int *)client_socket;
  L_DEB_THR("http", "handler", "Client socket: %i", sock);

  SET_TIMER

  // Read request from socket
  char req_raw[HTTP_REQUEST_MAX_LEN_BYTES];

  ssize_t req_raw_len =
      http_read_request(req_raw, HTTP_REQUEST_MAX_LEN_BYTES, sock);
  if (req_raw_len < 1) {
    L_ERR_THR("http", "handler", "Error while receiving request");

    http_internal_error(sock);
    http_close_safe(sock, 10);

    TIME_IT("handler", "");
    return NULL;
  }

  // Parse request string
  request req;
  if (http_parse_request(&req, req_raw) != HTTP_SUCCESS) {
    L_ERR_THR("http", "handler", "Error while parsing request");

    http_internal_error(sock);
    http_close_safe(sock, 10);

    TIME_IT("handler", "");
    return NULL;
  }

  // decode request
  if (http_decode_request(&req) != HTTP_SUCCESS) {
    L_ERR_THR("http", "handler", "Error while decoding request");
    TIME_IT("handler", "");
    return NULL;
  }

  if (REQ_TYPE_IS(HTTP_GET)) {
    // Get request
    if (http_get(sock, &req) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);

      TIME_IT("handler", "");

      return NULL;
    }
  } else if (REQ_TYPE_IS(HTTP_HEAD)) {
    // Head request
    if (http_head(sock, &req) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);
      TIME_IT("handler", "");

      return NULL;
    }
  } else {
    // Unknown type
    if (http_unknown_method(sock) != HTTP_SUCCESS) {
      http_internal_error(sock);
      http_close_safe(sock, 10);
      TIME_IT("handler", "");

      return NULL;
    }
  }

  http_close_safe(sock, 10);
  TIME_IT("handler", "");

  return NULL;
}