#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "task8.h"
#include "base_utils.h"

/* Перевод unsigned long long в строку в базе (2..36), БЕЗ знака */
static void to_base_str_ull(unsigned long long uv, int base, char *out, size_t outsz) {
    char tmp[256];
    int pos = 0;

    if (uv == 0ULL) {
        tmp[pos++] = '0';
    } else {
        while (uv > 0ULL && pos < (int)sizeof(tmp) - 1) {
            int d = (int)(uv % (unsigned)base);
            uv /= (unsigned)base;
            tmp[pos++] = (char)digit_to_char(d);
        }
    }

    size_t k = 0;
    for (int i = pos - 1; i >= 0 && k < outsz - 1; --i) out[k++] = tmp[i];
    out[k] = '\0';
}

status_t task8_run(void) {
    int base = 0;
    if (scanf("%d", &base) != 1 || base < 2 || base > 36) {
        fprintf(stderr, "Bad base\n");
        return ST_ERR_ARGS;
    }

    char buf[4096];
    char best[4096] = {0};
    int have = 0;

    /* Читаем числа строчно, окончание — слово "Stop" */
    while (scanf("%4095s", buf) == 1) {
        if (strcmp(buf, "Stop") == 0) break;
        if (!have) {
            strcpy(best, buf);
            have = 1;
        } else {
            int cmp = abs_compare_in_base(buf, best, base);
            if (cmp > 0) strcpy(best, buf);
        }
    }

    if (!have) {
        printf("\n");
        return ST_OK;
    }

    /* Печать максимума по модулю БЕЗ знака и без ведущих нулей */
    const char *p = best;
    int neg = (*p == '-');
    if (neg || *p == '+') ++p;
    size_t len = 0;
    while (p[len] && !isspace((unsigned char)p[len])) ++len;
    size_t newlen = trim_leading_zeros(&p, len);
    if (newlen == 0) p = "0";
    printf("%s\n", p);

    /* Перевод строки в long long; при ошибке — OVERFLOW 4 раза */
    long long val = 0;
    status_t st = to_int64_base(best, base, &val);
    if (st != ST_OK) {
        printf("OVERFLOW\nOVERFLOW\nOVERFLOW\nOVERFLOW\n");
        return ST_OK;
    }

    /* Работать дальше с МОДУЛЕМ как с unsigned long long (корректно для LLONG_MIN) */
    unsigned long long uv =
        (val < 0) ? (unsigned long long)(-(val + 1)) + 1ULL
                  : (unsigned long long)val;

    char out[256];
    to_base_str_ull(uv, 9,  out, sizeof(out)); printf("%s\n", out);
    to_base_str_ull(uv, 18, out, sizeof(out)); printf("%s\n", out);
    to_base_str_ull(uv, 27, out, sizeof(out)); printf("%s\n", out);
    to_base_str_ull(uv, 36, out, sizeof(out)); printf("%s\n", out);

    return ST_OK;
}
