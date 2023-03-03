#include "logger.h"

void logger(FILE* stream, const int log_level, const int flags,
            const char* pkg_name, const char* func_name, const char* fmt, ...) {
  if (!fmt) {
    return;
  }

  const log_level_t* p;
  for (p = log_levels; p->code && p->code != log_level; ++p)
    ;

  char* log_level_name = p->msg;
  if (!stream) {
    stream = *p->stream;
  }

  fflush(stream);

  if (log_level_name) {
    switch (log_level) {
      case LV_ERROR: {
        fprintf(stream, "\x1B[31m[%s]\033[0m ", log_level_name);
        break;
      }
      case LV_INFO: {
        fprintf(stream, "\x1B[34m[%s]\033[0m ", log_level_name);
        break;
      }
      default: {
        fprintf(stream, "[%s] ", log_level_name);
        break;
      }
    }
  }

  if (flags == FL_THREAD) {
    fprintf(stream, "[thread: %lu] ", pthread_self());
  }

  if (pkg_name) {
    fprintf(stream, "(pkg: \"%s\") ", pkg_name);
  }

  if (func_name) {
    fprintf(stream, "%s()", func_name);
  }

  fprintf(stream, "%s", ": ");
  if (strchr(fmt, '%')) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);
    fprintf(stream, "\n");
  } else {
    fprintf(stream, "%s\n", fmt);
  }

}
