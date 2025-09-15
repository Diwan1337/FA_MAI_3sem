#ifndef PARSE_H
#define PARSE_H

#include <stdint.h>
#include "status.h"

status_t parse_double_strict(const char *s, double *out);
status_t parse_int64(const char *s, int64_t *out);

#endif /* PARSE_H */