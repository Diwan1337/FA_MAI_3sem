#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include "parse.h"

static const char* skip_ws(const char* s){
    while (s && *s && isspace((unsigned char)*s)) ++s;
    return s;
}

static const char* eat_sign_run(const char* s, int* sign_out){
    int minus = 0;
    while (*s == '+' || *s == '-') { if (*s=='-') ++minus; ++s; }
    if (sign_out) *sign_out = (minus % 2 == 0) ? +1 : -1;
    return s;
}

status_t parse_double_strict(const char *s, double *out) {
    if (!s || !out) return ST_ERR_ARGS;

    s = skip_ws(s);
    int sign = +1;
    const char *p = eat_sign_run(s, &sign);

    /* strtod не понимает множественные знаки, поэтому подадим ему уже нормализованную строку */
    char *end = NULL;
    errno = 0;
    double v = strtod(p, &end);
    if (errno != 0) return ST_ERR_PARSE;
    if (end == p) return ST_ERR_PARSE;

    v = (sign == +1) ? v : -v;

    end = (char*)skip_ws(end);
    if (*end != '\0') return ST_ERR_PARSE;

    *out = v;
    return ST_OK;
}
