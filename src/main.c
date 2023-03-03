#include <stdlib.h>

#include "config_reader.h"
#include "http.h"
#include "logger.h"
#include "server.h"

int main(int argc, char **argv) {
  if (argc < 1) {
    L_ERR(NULL, "main", "Config file not provided");
    return EXIT_FAILURE;
  }
  static config_field conf[] = {{"thread_limit", T_INT, NULL},
                                {"connection_limit", T_INT, NULL},
                                {"port", T_INT, NULL},
                                {"server_name", T_STRING, NULL},
                                {"document_root", T_STRING, NULL},
                                {"dir_index", T_STRING, NULL},
                                {NULL, 0, NULL}};

  if (config_read(conf, argv[1]) != EXIT_SUCCESS) {
    L_ERR(NULL, "main", "Config file reading error");
    config_free(conf);
    return EXIT_FAILURE;
  }

  if (http_setup(str__get_val(conf, "server_name"),
                 str__get_val(conf, "document_root"),
                 str__get_val(conf, "dir_index")) != HTTP_SUCCESS) {
    L_ERR(NULL, "main", "Configuring http error");
    config_free(conf);
    return EXIT_FAILURE;
  }

  server *serv = server_init(int__get_val(conf, "port"),
                             int__get_val(conf, "connection_limit"),
                             int__get_val(conf, "thread_limit"), http_handler);

  config_free(conf);
  server_run(serv);
  server_destroy(serv);
  return EXIT_SUCCESS;
}