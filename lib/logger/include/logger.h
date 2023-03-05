#ifndef LOGGER_H_
#define LOGGER_H_

#define LOGGER

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int code;
  char* msg;
  FILE** stream;
} log_level_t;

enum LOG_FL { FL_THREAD = 1 };

enum LOG_LV { LV_DEBUG = 1, LV_ERROR, LV_INFO };

static const log_level_t log_levels[] = {{LV_DEBUG, "DEBUG", &stdout},
                                         {LV_ERROR, "ERROR", &stderr},
                                         {LV_INFO, "INFO", &stdout},
                                         {0, NULL, NULL}};

void logger(FILE* stream, const int log_level, const int flags,
            const char* pkg_name, const char* func_name, const char* fmt, ...);

#ifdef DEBUG

#define L_DEB(pkg, func, fmt, ...) \
  logger(NULL, LV_DEBUG, 0, pkg, func, fmt, ##__VA_ARGS__)
#define L_DEB_THR(pkg, func, fmt, ...) \
  logger(NULL, LV_DEBUG, FL_THREAD, pkg, func, fmt, ##__VA_ARGS__)

#else

#define L_DEB(...)
#define L_DEB_THR(...)

#endif

#ifdef NO_ERR

#define L_ERR(...)
#define L_ERR_THR(...)

#else

#define L_ERR(pkg, func, fmt, ...) \
  logger(NULL, LV_ERROR, 0, pkg, func, fmt, ##__VA_ARGS__)
#define L_ERR_THR(pkg, func, fmt, ...) \
  logger(NULL, LV_ERROR, FL_THREAD, pkg, func, fmt, ##__VA_ARGS__)

#endif

#ifdef NO_INFO

#define L_INFO(...)
#define L_INFO_THR(...)

#else

#define L_INFO(pkg, func, fmt, ...) \
  logger(NULL, LV_INFO, 0, pkg, func, fmt, ##__VA_ARGS__)
#define L_INFO_THR(pkg, func, fmt, ...) \
  logger(NULL, LV_INFO, FL_THREAD, pkg, func, fmt, ##__VA_ARGS__)

#endif

#endif  // LOGGER_H_
