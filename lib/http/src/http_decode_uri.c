#include "http.h"

int http_decode_uri(char *uri) {
  // DEB("decode: %s\n", uri);
  int idx = 0;
  char *ptr_sym = uri;
  char encoded_sym[] = "00";

  while (*ptr_sym != '\0') {
    if (*ptr_sym == '%') {
      encoded_sym[0] = *(++ptr_sym);
      encoded_sym[1] = *(++ptr_sym);

      long decoded_sym = strtol(encoded_sym, NULL, 16);
      uri[idx++] = decoded_sym;

    } else {
      uri[idx++] = *ptr_sym;
    }
    ++ptr_sym;
  }
  uri[idx] = '\0';
  DEB("\rDecoded: %s\n", uri);

  return HTTP_SUCCESS;
}