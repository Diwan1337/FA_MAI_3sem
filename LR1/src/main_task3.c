#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "common.h"
#include "task3.h"

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s <-q|-m|-t> [args...]\n"
        "  -q <eps> <a> <b> <c>  - решить квадратное уравнение ax²+bx+c=0\n"
        "  -m <a> <b>           - проверить кратность a к b\n"
        "  -t <eps> <a> <b> <c> - проверить прямоугольность треугольника\n",
        prog);
}

int main(int argc, char **argv) {
    if (argc < 2) { print_usage(argv[0]); return ST_ERR_ARGS; }
    
    const char *flag = argv[1];
    
    if (strcmp(flag, "-q") == 0) {
        if (argc != 6) { print_usage(argv[0]); return ST_ERR_ARGS; }
        double eps, a, b, c;
        if (sscanf(argv[2], "%lf", &eps) != 1 ||
            sscanf(argv[3], "%lf", &a) != 1 ||
            sscanf(argv[4], "%lf", &b) != 1 ||
            sscanf(argv[5], "%lf", &c) != 1) {
            return ST_ERR_PARSE;
        }
        return task3_q_solve_quad(eps, a, b, c);
    }
    else if (strcmp(flag, "-m") == 0) {
        if (argc != 4) { print_usage(argv[0]); return ST_ERR_ARGS; }
        int64_t a, b;
        if (parse_int64(argv[2], &a) != ST_OK ||
            parse_int64(argv[3], &b) != ST_OK) {
            return ST_ERR_PARSE;
        }
        return task3_m_is_multiple(a, b);
    }
    else if (strcmp(flag, "-t") == 0) {
        if (argc != 6) { print_usage(argv[0]); return ST_ERR_ARGS; }
        double eps, a, b, c;
        if (sscanf(argv[2], "%lf", &eps) != 1 ||
            sscanf(argv[3], "%lf", &a) != 1 ||
            sscanf(argv[4], "%lf", &b) != 1 ||
            sscanf(argv[5], "%lf", &c) != 1) {
            return ST_ERR_PARSE;
        }
        return task3_t_is_right_triangle(eps, a, b, c);
    }
    else {
        print_usage(argv[0]);
        return ST_ERR_ARGS;
    }
}
