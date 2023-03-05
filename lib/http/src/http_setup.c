#include "http.h"

int http_setup(const char *serv_name, const char *doc_root,
               const char *dir_idx) {
  http_config_t conf;
  L_INFO("http", "setup",
         "Configuring http: Server name \"%s\", Document root: %s, Directory "
         "index: %s",
         serv_name, doc_root, dir_idx);
  if (!memcpy(conf.server_name, serv_name,
              strlen(serv_name) < HTTP_SERV_NAME_MAX ? HTTP_SERV_NAME_MAX
                                                     : strlen(serv_name)) ||
      !memcpy(conf.document_root, doc_root,
              strlen(doc_root) < HTTP_PATH_MAX ? HTTP_PATH_MAX
                                               : strlen(doc_root)) ||
      !memcpy(conf.dir_index, dir_idx,
              strlen(dir_idx) < HTTP_INDEX_MAX ? HTTP_INDEX_MAX
                                               : strlen(dir_idx))) {
    L_ERR("http", "setup", "Can't configure HTTP: %s", strerror(errno));
    return HTTP_ERROR;
  }

  http_config(&conf);

  return HTTP_SUCCESS;
}
