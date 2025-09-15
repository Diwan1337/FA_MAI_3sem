#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

status_t parse_double_strict(const char *s, double *out) {
    if (!s || !out) return ST_ERR_ARGS;
    char *end = NULL;
    errno = 0;
    double v = strtod(s, &end);
    if (errno != 0) return ST_ERR_PARSE;
    if (end == s) return ST_ERR_PARSE;
    while (*end && isspace((unsigned char)*end)) ++end;
    if (*end != '\0') return ST_ERR_PARSE;
    *out = v;
    return ST_OK;
}
