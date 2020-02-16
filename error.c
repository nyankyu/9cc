#include <errno.h>
#include <string.h>
#include "error.h"

void error(char *fmt, ...) {
  va_list ap;
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void error_str(char *fmt, ...) {
  va_list ap;
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, " : errno=%d, %s\n", errno, strerror(errno));
  exit(1);
}

void error_at(char *user_input, char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

