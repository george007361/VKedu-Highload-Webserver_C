#ifndef LOGGER_H_
#define LOGGER_H_

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int code;
  char* msg;
  FILE** stream;
} log_level_t;

enum LOG_LEVEL_FLAGS { LOG_THREAD = 1 };

enum LOG_LEVEL_CODE { LOG_DEBUG = 1, LOG_ERROR, LOG_INFO };

static const log_level_t log_levels[] = {{LOG_DEBUG, "DEBUG", &stdout},
                                         {LOG_ERROR, "ERROR", &stderr},
                                         {LOG_INFO, "INFO", &stdout},
                                         {0, NULL, NULL}};

void LOG(FILE* stream, const int log_level, const int flags,
         const char* pkg_name, const char* func_name, const char* fmt, ...);

#endif  // LOGGER_H_