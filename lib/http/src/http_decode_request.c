#include "http.h"

int decode_string(char *str) {
  int idx = 0;
  char *ptr_sym = str;
  char encoded_sym[] = "00";

  L_DEB_THR("http", "decode_string", "Encoded string: %s", str);

  while (*ptr_sym != '\0') {
    if (*ptr_sym == '%') {
      encoded_sym[0] = *(++ptr_sym);
      encoded_sym[1] = *(++ptr_sym);

      long decoded_sym = strtol(encoded_sym, NULL, 16);
      str[idx++] = decoded_sym;

    } else {
      str[idx++] = *ptr_sym;
    }
    ++ptr_sym;
  }
  str[idx] = '\0';

  L_DEB_THR("http", "decode_string", "Decoded string: %s", str);

  return HTTP_SUCCESS;
}

int http_decode_request(request *req) {
  if (decode_string(req->uri) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_decode_request()]: Can't decode URI\n");
    return HTTP_ERROR;
  }

  if (decode_string(req->query) != HTTP_SUCCESS) {
    fprintf(stderr, "http[http_decode_request()]: Can't decode query\n");
    return HTTP_ERROR;
  }

  return HTTP_SUCCESS;
}