/***************************
 * File: src/main_task6.c
 ***************************/
#include <stdio.h>
#include "task6.h"
#include "parse.h"    // parse_double_strict
#include "status.h"   // коды возврата

static void usage(const char *p){
    fprintf(stderr,
        "Usage: %s <eps> <integral_id> <a> <b>\n"
        "  integral_id: 1->f1, 2->f2\n", p);
}

int main(int argc, char **argv){
    if (argc != 5) { usage(argv[0]); return ST_ERR_ARGS; }

    double eps = 0.0, a = 0.0, b = 0.0;
    int id = 0;

    // eps — строгое число, eps>0? иначе ST_ERR_ARGS
    if (parse_double_strict(argv[1], &eps) != ST_OK) return ST_ERR_PARSE;
    if (eps <= 0.0) return ST_ERR_ARGS;

    // id — целое; если не число, то это ошибка парсинга
    if (sscanf(argv[2], "%d", &id) != 1) return ST_ERR_PARSE;

    // a, b — строгие double; если не число — ST_ERR_PARSE
    if (parse_double_strict(argv[3], &a) != ST_OK) return ST_ERR_PARSE;
    if (parse_double_strict(argv[4], &b) != ST_OK) return ST_ERR_PARSE;

    // выбор интеграла; неверный id — ST_ERR_ARGS
    func_t f = NULL;
    if (id == 1) f = f1;
    else if (id == 2) f = f2;
    else { usage(argv[0]); return ST_ERR_ARGS; }

    // интегрируем (функция сама корректно обрабатывает a>b и a==b)
    double val = integrate_trapezoid(f, a, b, eps);
    printf("%.15g\n", val);
    return ST_OK;
}
