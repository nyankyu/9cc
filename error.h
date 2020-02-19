#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void error(char *fmt, ...);
void error_str(char *fmt, ...);

#endif

