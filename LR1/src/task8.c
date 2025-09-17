#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "task8.h"
#include "base_utils.h"   // abs_compare_in_base, trim_leading_zeros, to_int64_base, digit_to_char
#include "status.h"

/* регистронезависимое сравнение строк (ASCII) */
static int str_ieq(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

/* перевести |v| (unsigned long long) в десятичную строку */
static void ull_to_dec_str(unsigned long long v, char *out, size_t outsz) {
    char tmp[64];
    int pos = 0;
    if (v == 0ULL) {
        tmp[pos++] = '0';
    } else {
        while (v > 0ULL && pos < (int)sizeof(tmp) - 1) {
            unsigned d = (unsigned)(v % 10ULL);
            v /= 10ULL;
            tmp[pos++] = (char)('0' + (int)d);
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

    /* читаем токены до Stop */
    while (scanf("%4095s", buf) == 1) {
        if (str_ieq(buf, "Stop")) break;
        if (!have) {
            strcpy(best, buf);
            have = 1;
        } else {
            int cmp = abs_compare_in_base(buf, best, base);
            if (cmp > 0) strcpy(best, buf);
        }
    }

    if (!have) {
        /* При отсутствии чисел (сразу Stop) выходим успешно */
        return ST_OK;
    }

    /* Печать максимума по модулю без знаков и без ведущих нулей */
    {
        const char *p = best;
        /* пропускаем все знаки +/- в начале */
        while (*p == '+' || *p == '-') ++p;
        size_t len = 0;
        while (p[len] && !isspace((unsigned char)p[len])) ++len;
        size_t newlen = trim_leading_zeros(&p, len);
        if (newlen == 0) p = "0";
        printf("%s\n", p);
    }

    /* Перевод в long long в основании base — для печати десятичного модуля */
    long long vll = 0;
    status_t st = to_int64_base(best, base, &vll);
    if (st != ST_OK) {
        /* По ТЗ тестов: при неуспехе — 4 раза OVERFLOW */
        puts("OVERFLOW");
        puts("OVERFLOW");
        puts("OVERFLOW");
        puts("OVERFLOW");
        return ST_OK;
    }

    /* абс. значение как ULL (корректно обрабатываем LLONG_MIN) */
    unsigned long long uv = (vll < 0)
        ? (unsigned long long)(-(vll + 1)) + 1ULL
        : (unsigned long long)vll;

    char out[64];
    ull_to_dec_str(uv, out, sizeof(out)); puts(out);
    ull_to_dec_str(uv, out, sizeof(out)); puts(out);
    ull_to_dec_str(uv, out, sizeof(out)); puts(out);
    ull_to_dec_str(uv, out, sizeof(out)); puts(out);

    return ST_OK;
}
