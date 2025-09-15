#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include "common.h"

status_t parse_int64(const char *s, int64_t *out) {
    if (!s || !out) return ST_ERR_ARGS;
    const char *p = s;
    while (isspace((unsigned char)*p)) ++p;
    int sign = 1;
    if (*p == '+') { ++p; }
    else if (*p == '-') { sign = -1; ++p; }
    if (!isdigit((unsigned char)*p)) return ST_ERR_PARSE;

    int64_t val = 0;
    for (; *p; ++p) {
        if (isspace((unsigned char)*p)) {
            const char *q = p;
            while (isspace((unsigned char)*q)) ++q;
            if (*q != '\0') return ST_ERR_PARSE;
            break;
        }
        if (!isdigit((unsigned char)*p)) return ST_ERR_PARSE;
        int d = *p - '0';
        if (sign == 1) {
            if (val > (INT64_MAX - d) / 10) return ST_ERR_OVERFLOW;
            val = val * 10 + d;
        } else {
            if (-val < (INT64_MIN + d) / 10) return ST_ERR_OVERFLOW;
            val = val * 10 + d;
        }
    }
    *out = (sign == 1) ? val : -val;
    return ST_OK;
}

status_t safe_sum_1_to_n(int64_t n, int64_t *out) {
    if (!out) return ST_ERR_ARGS;
    if (n < 0) return ST_ERR_RANGE;
    // Вычисляем n*(n+1)/2 без __int128 и без переполнения
    int64_t a, b;
    if ((n & 1) == 0) { // n чётное: (n/2)*(n+1)
        a = n / 2;
        b = n + 1;
    } else { // n нечётное: ((n+1)/2)*n
        a = (n + 1) / 2;
        b = n;
    }
    if (a != 0 && b > INT64_MAX / a) return ST_ERR_OVERFLOW;
    *out = a * b;
    return ST_OK;
}

status_t safe_factorial_u64(uint64_t n, uint64_t *out) {
    if (!out) return ST_ERR_ARGS;
    uint64_t r = 1;
    for (uint64_t i = 2; i <= n; ++i) {
        if (r > UINT64_MAX / i) return ST_ERR_OVERFLOW;
        r *= i;
    }
    *out = r;
    return ST_OK;
}

bool is_prime_u64(uint64_t x) {
    if (x <= 1) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}
