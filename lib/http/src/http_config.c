#include "http.h"

http_config_t http_config(http_config_t *set) {
  static http_config_t conf = {0};
  static int init = 0;

  if (set) {
    assert(!init);
    conf = *set;
    init = 1;
  }

  assert(init);
  return conf;  // Copy on value
}