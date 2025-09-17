#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include "task4.h"
#include "status.h"

static void usage(const char *p){
    fprintf(stderr,
        "Usage: %s <const> <method> <eps>\n"
        " const: e|pi|ln2|sqrt2|gamma\n"
        " method: series|equation|limit\n", p);
}

static void strtolower_inplace(char *s){
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

static status_t parse_eps_strict(const char *s, double *out){
    if (!s || !out) return ST_ERR_ARGS;
    errno = 0;
    char *end = NULL;
    double v = strtod(s, &end);
    if (errno != 0 || end == s) return ST_ERR_PARSE;      // не число
    while (*end && isspace((unsigned char)*end)) ++end;   // хвостовые пробелы
    if (*end != '\0') return ST_ERR_PARSE;                // мусорный хвост
    if (!(v > 0)) return ST_ERR_ARGS;                     // eps должен быть > 0
    *out = v;
    return ST_OK;
}

static method_t parse_method_ci(const char *s){
    char buf[32];
    snprintf(buf, sizeof(buf), "%s", s ? s : "");
    strtolower_inplace(buf);
    if (strcmp(buf,"series")==0)   return M_SERIES;
    if (strcmp(buf,"equation")==0) return M_EQUATION;
    return M_LIMIT; // по умолчанию limit (как и раньше), но ниже проверим валидность
}

static int is_valid_method(const char *s){
    char buf[32];
    snprintf(buf, sizeof(buf), "%s", s ? s : "");
    strtolower_inplace(buf);
    return (strcmp(buf,"series")==0 || strcmp(buf,"equation")==0 || strcmp(buf,"limit")==0);
}

static int const_kind_ci(const char *s){ // 1=e,2=pi,3=ln2,4=sqrt2,5=gamma, 0=bad
    char buf[32];
    snprintf(buf, sizeof(buf), "%s", s ? s : "");
    strtolower_inplace(buf);
    if (strcmp(buf,"e")==0)     return 1;
    if (strcmp(buf,"pi")==0)    return 2;
    if (strcmp(buf,"ln2")==0)   return 3;
    if (strcmp(buf,"sqrt2")==0) return 4;
    if (strcmp(buf,"gamma")==0) return 5;
    return 0;
}

int main(int argc, char **argv){
    if (argc != 4) { usage(argv[0]); return ST_ERR_ARGS; }

    // нормализуем регистр и валидируем метод/константу
    if (!is_valid_method(argv[2])) { usage(argv[0]); return ST_ERR_ARGS; }
    method_t m = parse_method_ci(argv[2]);

    int ckind = const_kind_ci(argv[1]);
    if (ckind == 0) { usage(argv[0]); return ST_ERR_ARGS; }

    double eps = 0.0;
    status_t pst = parse_eps_strict(argv[3], &eps);
    if (pst != ST_OK) return pst; // ST_ERR_PARSE (2) для мусора, ST_ERR_ARGS (1) для eps<=0

    double out = 0.0;
    status_t st = ST_OK;
    switch (ckind){
        case 1: st = calc_e(eps, m, &out);     break;
        case 2: st = calc_pi(eps, m, &out);    break;
        case 3: st = calc_ln2(eps, m, &out);   break;
        case 4: st = calc_sqrt2(eps, m, &out); break;
        case 5: st = calc_gamma(eps, m, &out); break;
        default: usage(argv[0]); return ST_ERR_ARGS;
    }
    if (st != ST_OK) return st;
    printf("%.15g\n", out);
    return 0;
}
