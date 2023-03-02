#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Supporting types
enum CONF_VAL_TYPES { T_INT = 0, T_STRING };

// Settings
#define CONF_STR_MAX_LEN 128

// Config structure
typedef struct config_field {
  const char *name;
  const int arg_type;
  void *val;
} config_field;

// Read values from file
int config_read(config_field *conf, char *path);

// Free memory after use
void config_free(config_field *conf);

// Methods to get field name
const char *get_name_index(config_field *conf, int idx);
const char *get_name(config_field *conf, const char *key);

// Methods to get values

// For INT type
int int__get_val(config_field *conf, const char *key);
int int__get_val_index(config_field *conf, int idx);

// For STRING type
char *str__get_val(config_field *conf, const char *key);
char *str__get_val_index(config_field *conf, int idx);

#endif  // CONFIG_READER_H_