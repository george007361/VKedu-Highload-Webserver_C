#include "config_reader.h"

void config_free(config_field *conf) {
  if (!conf) {
    return;
  }

  for (config_field *p = conf; p->name; ++p) {
    if (p->val) {
      free(p->val);
    }
  }
}

int config_read(config_field *conf, char *path) {
  FILE *file = fopen(path, "rt");
  if (!file) {
    return EXIT_FAILURE;
  }

  while (!feof(file)) {
    char buff[CONF_STR_MAX_LEN];
    if (!fgets(buff, CONF_STR_MAX_LEN - 1, file) && !feof(file)) {
      fclose(file);
      return EXIT_FAILURE;
    }

    if (buff[0] == '#') {
      continue;
    }

    char *sep = strchr(buff, ' ');
    size_t name_len = sep - buff;

    char *end = strchr(sep + 1, ' ');
    if (!end) {
      end = strchr(sep + 1, '\n');
    }
    if (!end) {
      end = strchr(sep + 1, '\0');
    }
    if (!end) {
      return EXIT_FAILURE;
    }

    *end = '\0';
    size_t val_len = end - sep - 1;

    config_field *p;
    for (p = conf; p->name && strncmp(buff, p->name, name_len); ++p)
      ;

    if (!p->name) {
      continue;
    }

    switch (p->arg_type) {
      case T_INT: {
        p->val = malloc(sizeof(int));
        if (!p->val) {
          return EXIT_FAILURE;
        }

        int val = atoi(sep + 1);
        if (!memcpy(p->val, &val, sizeof(int))) {
          return EXIT_FAILURE;
        }

        break;
      }

      case T_STRING: {
        p->val = malloc(val_len * sizeof(char));
        if (!p->val) {
          return EXIT_FAILURE;
        }

        if (!memcpy(p->val, sep + 1, val_len)) {
          return EXIT_FAILURE;
        }
      }

      default:
        break;
    }
  }
  fclose(file);
  return EXIT_SUCCESS;
}

const char *get_name_index(config_field *conf, int idx) {
  assert(conf);
  assert(conf[idx].name);
  return conf[idx].name;
}

char *str__get_val_index(config_field *conf, int idx) {
  assert(conf);
  assert(conf[idx].name);
  assert(conf[idx].arg_type == T_STRING);
  return (char *)conf[idx].val;
}

int int__get_val_index(config_field *conf, int idx) {
  assert(conf);
  assert(conf[idx].name);
  assert(conf[idx].arg_type == T_INT);
  return *(int *)conf[idx].val;
}

const char *get_name(config_field *conf, const char *key) {
  assert(conf);
  assert(key);
  int i;
  for (i = 0; conf[i].name && strcmp(conf[i].name, key); ++i)
    ;
  return get_name_index(conf, i);
}

char *str__get_val(config_field *conf, const char *key) {
  assert(conf && key);
  int i;
  for (i = 0; conf[i].name && strcmp(conf[i].name, key); ++i)
    ;
  return str__get_val_index(conf, i);
}

int int__get_val(config_field *conf, const char *key) {
  assert(conf && key);
  int i;
  for (i = 0; conf[i].name && strcmp(conf[i].name, key); ++i)
    ;
  return int__get_val_index(conf, i);
}
